{
   "variables": {
       "NODE_VERSION%":"<!(node -p \"process.versions.node.split(\\\".\\\")[0]\")"
   },
  "targets": [
    {
      "target_name": "racf",
      "sources": [ "racf_interface.cpp", "racf.cpp" ],
      "include_dirs": [
         "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "conditions": [
        [ "NODE_VERSION < 18", {
          "cflags": [  "-qascii" ],
          "cflags_cc": [ "-qascii" ]
        }],
      ],
    }
  ]
}
