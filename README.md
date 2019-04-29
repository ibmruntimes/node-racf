# RACF
This NodeJS module enables you validate against RACF

## Installation

<!--
This is a [Node.js](https://nodejs.org/en/) module available through the
[npm registry](https://www.npmjs.com/).
-->

Before installing, [download and install Node.js](https://developer.ibm.com/node/sdk/ztp/).
Node.js 6.14 for z/OS or higher is required.

## Setup

In order to use this module, you must Set the Program Control bit on Node and its dependent DLLs.

```bash
extattr +p
```

A convenience script, `setup.sh` can do this for you automatically

## Simple to use

### Install

```bash
npm install racf
```

### Use

```js
const racf = require("racf");

// Check if user belongs to a group
if (racf.isUserInGroup(process.env.RACF_TEST_ID, "DEV")) {
	try {
		// Authenticate user and password against RACF
        var isSuccessful = racf.authenticate(process.env.RACF_TEST_ID, process.env.RACF_TEST_PASSWORD);
        console.log(isSuccessful);
	} catch(err) {
		console.log.out(err);
	}
}
```

