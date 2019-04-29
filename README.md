# RACF
This NodeJS module enables you validate against RACF

## Installation

<!--
This is a [Node.js](https://nodejs.org/en/) module available through the
[npm registry](https://www.npmjs.com/).
-->

Before installing, [download and install Node.js](https://developer.ibm.com/node/sdk/ztp/).
Node.js 6.11.4 or higher is required.

## Setup

In order to use this module, you must Set the Program Control bit on Node and its dependent DLLs.

```
extattr +p
```

A convenience script, setup.sh can do this for you automatically if you set the $NODE_INSTALL_DIR environment variable.

## Simple to use

### Install

npm install racf

### Use

const racf = require("racf");

try {
	racf.authenticate(process.env.RACF_TEST_ID, process.env.RACF_TEST_PASSWORD);
} catch(err) {
	console.log.out(err);
}
