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

describe("Key Sequenced Dataset", function() {
  it("check racf authenticate to be valid", function(done) {
    expect(racf.authenticate(process.env.RACF_TEST_ID, process.env.RACF_TEST_PASSWORD)).to.be.true;
    done();
  });

  it("check racf authenticate to be invalid", function(done) {
    expect(racf.authenticate(process.env.RACF_TEST_ID, "BADPASSWORD")).to.be.false;
    done();
  });
});
