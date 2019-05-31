/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2017. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

const racf = require("../build/Release/racf.node");
const async = require('async');
const fs = require('fs');
const expect = require('chai').expect;
const should = require('chai').should;
const assert = require('chai').assert;

describe("RACF Validation", function() {
  it("check racf authentication to be valid", function(done) {
    expect(racf.authenticate(process.env.RACF_TEST_ID, process.env.RACF_TEST_PASSWORD)).to.be.true;
    done();
  });

  it("check racf authentication to be invalid", function(done) {
    expect(racf.authenticate(process.env.RACF_TEST_ID, "BADPASSWORD")).to.be.false;
    done();
  });
  it("check if user does not belong to group", function(done) {
    expect(racf.isUserInGroup(process.env.RACF_TEST_ID, process.env.RACF_TEST_NOTINGROUP)).to.be.false;
    done();
  });

  it("check if user belong to group", function(done) {
    expect(racf.isUserInGroup(process.env.RACF_TEST_ID, process.env.RACF_TEST_INGROUP)).to.be.true;
    done();
  });

});
