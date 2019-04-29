{
  "targets": [
    {
      "target_name": "racf",
      "sources": [ "racf_interface.cpp", "racf.cpp" ],
	  "include_dirs" : [
    "<!(node -e \"require('nan')\")"
]
    }
  ]
}
