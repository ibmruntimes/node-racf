#include <stdio.h>

#pragma linkage(IRRSDL64, OS)

// Parameter List
struct Parm_list {
	struct {
		int dbToken;
		int Number_predicates;
		int Attribute_ID;
		int Attribute_length;
		int* Attribute_ptr;
	} ResultsHandle;
	int Certificate_Usage;
	int Default;
	int Certificate_length;
	int* Certificate_ptr;
	int Private_key_length;
	int* Private_key_ptr;
	int Private_key_type;
	int Private_key_bitsize;
	int Label_length;
	int* Label_ptr;
	char CERT_user_ID[9];
	int Subjects_DN_length;
	int Subjects_DN_ptr;
	int Record_ID_length;
	int Record_ID_ptr;
};

struct RACF_user_ID {
	char length;
	char name[8];
};

struct RACF_ring_name {
	char length;
	char name[237];
};

int R_datalib() {
	int workarea[1024];
	short ALET = 0;
	int SAF_return_code;
	int RACF_return_code;
	int RACF_reason_code;
	char Function_code = 0x01;
	int Attributes = 0x80000000;
	struct RACF_user_ID user_id;
	user_id.length = 7;
    memcpy(user_id.name, "ITODORO", 7);

	struct RACF_ring_name ring_name;
	ring_name.length = 7;
    memcpy(ring_name.name, "ITODORO", 7);
	
	int Parm_list_version = 0;
	struct Parm_list Parmlist;

	//Parmlist.ResultsHandle.Number_predicates = 0x1;
	Parmlist.ResultsHandle.Number_predicates = 0x0;
	Parmlist.ResultsHandle.Attribute_ID = 0x3; // Attribute data to match on is the DER-encoded subject's distinguished name.
	Parmlist.ResultsHandle.Attribute_length = 5;
	Parmlist.ResultsHandle.Attribute_ptr = "DATA";
	Parmlist.Label_ptr = "DATA";
	Parmlist.Label_length = 5;
	char record[1024];
	Parmlist.Record_ID_length = 1024;
	Parmlist.Record_ID_ptr = &record[0];;
	
    int Num_parms = 14;

    IRRSDL64(&Num_parms,
          &workarea[0],
          ALET, &SAF_return_code,
          ALET, &RACF_return_code,
          ALET, &RACF_reason_code,
          &Function_code,
          &Attributes,
          &user_id,
          &ring_name,
          &Parm_list_version,
          &Parmlist
         );

     printf("%d - %d - %d\n", SAF_return_code, RACF_return_code, RACF_reason_code);
}

int create_new_ring() {
	int workarea[1024];
	short ALET = 0;
	int SAF_return_code;
	int RACF_return_code;
	int RACF_reason_code;
	char Function_code = 0x07;
	int Attributes = 0x80000000;
	struct RACF_user_ID user_id;
	user_id.length = 7;
    memcpy(user_id.name, "ITODORO", 7);

	struct RACF_ring_name ring_name;
	ring_name.length = 7;
    memcpy(ring_name.name, "ITODORO", 7);
	
	int Parm_list_version = 0;
	struct Parm_list Parmlist;

	//Parmlist.ResultsHandle.Number_predicates = 0x1;
	Parmlist.ResultsHandle.Number_predicates = 0x0;
	Parmlist.ResultsHandle.Attribute_ID = 0x3; // Attribute data to match on is the DER-encoded subject's distinguished name.
	Parmlist.ResultsHandle.Attribute_length = 5;
	Parmlist.ResultsHandle.Attribute_ptr = "DATA";
	Parmlist.Label_ptr = "DATA";
	Parmlist.Label_length = 5;
	char record[1024];
	Parmlist.Record_ID_length = 1024;
	Parmlist.Record_ID_ptr = &record[0];;
	
     int Num_parms = 14;

     IRRSDL64 (&Num_parms,
                    &workarea[0],
                    ALET, &SAF_return_code,
                    ALET, &RACF_return_code,
                    ALET, &RACF_reason_code,
                    &Function_code,
                    &Attributes,
                    &user_id,
                    &ring_name,
                    &Parm_list_version,
                    &Parmlist
                   );

     printf("%d - %d - %d\n", SAF_return_code, RACF_return_code, RACF_reason_code);
}

int main() {
	create_new_ring();
}
