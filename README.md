# RACF
This NodeJS module enables your application to validate against RACF

## Installation

<!--
This is a [Node.js](https://nodejs.org/en/) module available through the
[npm registry](https://www.npmjs.com/).
-->

Before installing, [download and install Node.js](https://developer.ibm.com/node/sdk/ztp/).
Node.js 6.14 for z/OS or higher is required.

## Setup

In order to use this module, you must set the Program Control bit on Node and its dependent DLLs.

```bash
extattr +p
```

A convenience script, `setup.sh` can do this for you automatically

```bash
./setup.sh
```

## Simple to use

### Install

```bash
npm install racf
```

### Use

```js
const racf = require("racf");

// Check if user belongs to a group
if (racf.isUserInGroup("myuserid", "DEV")) {
	try {
		// Authenticate user and password against RACF
        var isSuccessful = racf.authenticate("myuserid", "mypassword");
        console.log(isSuccessful);
	} catch(err) {
		console.log.out(err);
	}
}
```

### Use
Prior to running the tests, make sure to set the RACF_TEST_ID, RACF_TEST_PASSWORD, RACF_TEST_INGROUP and RACF_TEST_NOTINGROUP environment variables.
```bash
RACF_TEST_ID="MYUSERID" RACF_TEST_PASSWORD="MYPASSWORD" RACF_TEST_INGROUP="DEV_REALGROUP" RACF_TEST_NOTINGROUP="DEVFAKEGROUP" npm test
```
