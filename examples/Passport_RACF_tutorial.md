# Form-based authentication using Passport and RACF Node.js npms


**IBM SDK for Node.js - z/OS®** is a Javascript runtime that provides a secure, module-driven, highly scalable approach to accelerate digital transformation on IBM Z.


This tutorial will demonstrate how to create a form-based authentication using the [RACF](https://www.npmjs.com/package/racf) npm module and Passport [Passport](https://www.npmjs.com/package/passport) npm module.


## Prerequisites


### Download and Install IBM SDK for Node.js - z/OS®


Download IBM SDK for Node.js - z/OS® here: [Download IBM SDK for Node.js - z/OS®](https://www.ibm.com/us-en/marketplace/sdk-nodejs-compiler-zos)


Follow the [Knowledge Center Documentation installation instructions](https://www.ibm.com/support/knowledgecenter/SSTRRS_8.0.0/com.ibm.nodejs.zos.v8.doc/install.htm) to install Node.js on z/OS.


## Introduction to the z/OS RACF npm Module


### Step 1: Start a New Project


Start a terminal session and connect to your z/OS UNIX System Services (USS) environment.  
Create a directory named `my-racf-project` for your project and change your current working directory to it.  


```
mkdir my-racf-project
cd my-racf-project
```


Use npm to create a Node.js package as follows:


```
npm init --yes
```


This command will create a `package.json` file which describes your package and its dependencies. The file will contain the following contents:


```
{
  "name": "my-racf-project",
  "version": "1.0.0",
  "description": "",
  "main": "index.js",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "keywords": [],
  "author": "",
  "license": "ISC"
}
```


The main source file for our project will be `index.js` by default.


### Step 2: Install the RACF npm dependency


The next step is to install the [RACF](https://www.npmjs.com/package/RACF) native npm module as a dependency for our project as follows:


```
npm install racf
```


This command will generate a new file `package-lock.json`, as well as modify `package.json` to add RACF as a project dependency.  The RACF npm module, as well as all of its dependencies, will be downloaded and installed under a newly created `node_modules` directory.


In order to use the RACF npm module, you must set the [program control bit](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.cdsa100/cdsa1a1004.htm) on the node executable, node dlls, and native addon dlls.


A convenience script, `setup.sh`, located under the `node_modules/racf` directory can be used to set the program control bit automatically.  Invoke it as follows:
```
./node_modules/racf/setup.sh
```
You should receive the following output:
```
+ find . -name racf.node -exec extattr +p {} ;
+ [ -e /home/itodoro/install_test/node-v8.16.0-os390-s390x/bin/../lib/libnode.so ]
+ extattr +p /home/itodoro/install_test/node-v8.16.0-os390-s390x/bin/../lib/libnode.so
+ [ -e /home/itodoro/install_test/node-v8.16.0-os390-s390x/bin/../bin/node ]
+ extattr +p /home/itodoro/install_test/node-v8.16.0-os390-s390x/bin/../bin/node
+ [ -e CXXRT64 ]
```
If you receive the following error:
```
chattr() error: rv=-1, errno=8B, rsn=0924041B
```
It may indicate that you do not have READ access to the BPX.FILEATTR.PROGCTL resource in the FACILITY class.  This issue can be solved by providing access to BPX.FILEATTR.PROGCTL for your userid.


### Step 3: Using the RACF npm


We will now leverage the RACF npm to authenticate a userid with a given password.  In addition, we will verify that the supplied userid belongs to a given group.


Create a new file named `index.js`, and open it in a text editor of your choosing.  


Write the following code to the file:


```javascript
// Import the RACF npm module
const racf = require("racf");
// Check if user belongs to a RACF group
function authenticateUser(userid, password, group) {
  if (racf.isUserInGroup(userid, group)) {
    // Authenticate user and password against RACF
    var isSuccessful = racf.authenticate(userid, password);
    return isSuccessful;
  }
  return false;
}


var isSuccessful = authenticateUser("MYUSERID", "MYPASSWORD", "MYGROUP");
console.log("Authentication is " + (isSuccessful ? "successful" : "unsuccessful"));
```


The above source code begins by importing the RACF npm module.   It then defines a function `authenticateUser`, which takes 3 parameters: `userid`, `password`, and `group`.  

Before we describe the logic in the function authenticateUser, let's take a closer look at the two APIs provided by RACF npm module:

`isUserInGroup(userid, group)` - This function validates if a userid belongs to a RACF group

and

`authenticate(userid, password)` - This function authenticates a RACF userid with a given password


In `authenticateUser`, we use the function `isUserInGroup` to validate that the supplied userid belongs to the group id passed into the function.  If the userid belongs to the group, we then authenticate the userid with the password provided.  


Finally, a call to `authenticateUser` is performed.  Make sure to modify the line `authenticateUser("MYUSERID", "MYPASSWORD", "MYGROUP");` to match your userid and password credentials, as well as your group id.


### Step 4: Run the RACF npm example


Run the `index.js` code with the Node.js runtime using the following command:


```
node index.js
```


If the credentials provided were valid, you should see the following message appear:
```
Authentication is successful
```


## RACF Authentication with the Passport module


Now, let's use what we've learned with the RACF npm module to create a web-based form that authenticates locally against RACF.  


### Step 1: Install the Passport, Passport-Local, Body-parser and Express npm dependencies


In this step, we will install 5 additional npm dependencies.


* The [Passport](https://www.npmjs.com/package/passport) npm module.   The Passport npm is used to authenticate requests, which it does through an extensible set of plugins known as strategies.  It has the added benefit of maintaining persistent login sessions.  
* The [passport-local](https://www.npmjs.com/package/passport-local) npm exposes the local authentication strategy to be used in our web-based form.  We will use this strategy to implement RACF authentication.
* The [express](https://www.npmjs.com/package/express) npm exposes a web framework that will be used to serve our web form.
* The [express-session](https://www.npmjs.com/package/express-session) npm handles and stores session data on the server-side.
* The [body-parser](https://www.npmjs.com/package/body-parser) npm enables the processing of HTTP POST requests


These npms can be installed as dependencies to your existing package as follows:


```
npm install passport passport-local express express-session body-parser
```


The above command will modify the file `package.json` to add passport, passport-local, express, express-session, and body-parser as project dependencies.


### Step 2: Creating a web form that authenticates against RACF


The Passport npm supports 300+ authentication strategies, such as username/password and OAuth.  In this tutorial, we will implement a username and password strategy.  The username and password strategy is exposed in the passport-local npm and we will use it to authenticate a userid and password against RACF.   The Express framework will be used to serve our web form


Create a new file named `passport.js`, and open it in a text editor of your choosing.  


Write the following code to the file:


```javascript
/var express = require('express');
var passport = require('passport');
var Strategy = require('passport-local').Strategy;
var racf = require('racf');

// Configure the local strategy for use by Passport.
//
// The local strategy require a `verify` function which receives the credentials
// (`username` and `password`) submitted by the user.  The function must verify
// that the password is correct and then invoke `cb` with a user object, which
// will be set at `req.user` in route handlers after authentication.
passport.use(new Strategy(
  function(username, password, cb) {
    try {
        isAuthenticated = racf.authenticate(username, password);
        if (!isAuthenticated)
            return cb(null, false);
        else
            return cb(null, username);
    } catch(err) {
        return cb(err);
    }
  }));


// Configure Passport authenticated session persistence.
//
// In order to restore authentication state across HTTP requests, Passport needs
// to serialize users into and deserialize users out of the session.  The
// typical implementation of this is as simple as supplying the user ID when
// serializing, and querying the user record by ID from the database when
// deserializing.
passport.serializeUser(function(user, cb) {
  cb(null, user);
});


passport.deserializeUser(function(user, cb) {
  cb(null, user);
});


// Create a new Express application.
var app = express();


// Use application-level middleware for common functionality, including
// POST request parsing, and session handling.
app.use(require('body-parser').urlencoded({ extended: true }));
app.use(require('express-session')({ secret: 'keyboard cat', resave: false, saveUninitialized: false }));


// Initialize Passport and restore authentication state, if any, from the
// session.
app.use(passport.initialize());
app.use(passport.session());


// Define routes.
app.get('/', (req, res) => res.sendFile('form.html', { root : __dirname}));


app.post('/login',
  passport.authenticate('local', { failureRedirect: '/' }),
  function(req, res) {
    res.redirect('/success');
  });


app.get('/success',
  function(req, res){
    res.send("Welcome " + req.user+"!!")
  });


const port = 3000;
app.listen(port , () => console.log('App listening on port ' + port));
```


The source code begins by importing the dependency modules.  The RACF authentication logic is implemented as a new username and password Passport strategy, with the resulting strategy passed to passport.use().   The functions `passport.serializeUser` and `passport.deserializeUser` are implemented for session persistence and in our implementation we use the userid as is.  The remainder of the code defines the GET and POST routes.   On the root (`'/'`) URI, we display the html form.   If the credentials provided are valid, we redirect the browser to `'/success'` URI, otherwise we redirect back to the root (`'/'`) URI.


The next step is to create an html file, which will be used to display the form.  Name the file `form.html`, and copy and paste the following contents to it:
```
<form action="/login" method="post">
    <div>
        <label>Username:</label>
        <input type="text" name="username"/>
    </div>
    <div>
        <label>Password:</label>
        <input type="password" name="password"/>
    </div>
    <div>
        <input type="submit" value="Log In"/>
    </div>
</form>
```
The form has two text inputs: `username` and `password` and one input button that is used to submit the form.  Once the `Log in` button is pressed, the form will redirect to the '/login' URI.


### Step 4: Run the Passport npm example


Run the `passport.js` code with the Node.js runtime using the following command:

```
node passport.js
```

Our web server will begin listening on port 3000.   You can now direct your web browser to port 3000 on the IP address or domain name of the server you are running this code on.  You will be presented with the following form:


Enter your credentials and press `Log in`.   The server will now authenticate your userid and password credentials against RACF.  If successful, you will be redirected to a "Welcome" page:
Congratulations!
