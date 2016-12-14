{
  "variables": {
    "cwd": "<!(node -e \"console.log(require('path').resolve(__dirname))\")",        
  },  
  "targets": [
    {
      "target_name": "krb5",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "src/krb5.cc"
      ],
      "conditions": [
        [
          "OS=='win'",
          {            
            "include_dirs": [
              "Kerberos/include",
              "Kerberos/include/krb5"
	          ],
            "conditions": [
              [
                "target_arch=='x64'",
                {                  
                  "link_settings": {
                    "libraries": [
                      "<(cwd)/Kerberos/lib/amd64/krb5_64.lib"
                    ]
                  },
                  "copies": [
                    {
                      "destination": "<(PRODUCT_DIR)",
                      "files": [
                        "Kerberos/bin/krb5_64.dll",
                        "Kerberos/bin/comerr64.dll",
                        "Kerberos/bin/k5sprt64.dll",
                        "Kerberos/bin/wshelp64.dll",
                        "vcredist/msvcp100.dll",
                        "vcredist/msvcr100.dll"
                      ]
                    }
                  ]
                }
              ],
              [
                "target_arch=='ia32'",
                {
                  "link_settings": {
                    "libraries": [
                      "$(MITKRB5)/Kerberos/lib/i386/krb5_32.lib"
                    ]
                  }
                }
              ]
            ]
          }
        ],
        [
          "OS!='win'",
          {
            "include_dirs": [
              "Kerberos/include",
              "Kerberos/include/krb5"
	    ],
            "link_settings": {
              "libraries": [
                "-lkrb5"
              ]
            }
          }
        ]
      ]
    }
  ]
}

