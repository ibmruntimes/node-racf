{
  "targets": [
    {
      "target_name": "racf",
      "sources": [ "racf_interface.cpp", "racf.cpp" ],
      "include_dirs": [
         "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "cflags": [  "-qascii" ],
      "cflags_cc": [  "-qascii" ]
    }
  ]
}
