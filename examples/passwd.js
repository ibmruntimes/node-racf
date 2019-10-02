/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2017. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

const racf = require("racf");

if (racf.isUserInGroup(process.env.RACF_TEST_ID, "CDEV")) {
    try {
		var isSuccessful = racf.authenticate(process.env.RACF_TEST_ID, process.env.RACF_TEST_PASSWORD);
		console.log(isSuccessful);
    } catch(err) {
        console.log(err);
    }
}
