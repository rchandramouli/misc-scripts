#ifndef TLV_H
#define TLV_H

/*
TLV_T_TOPOLOGY (TLV_T_SW_NAME) (TLV_T_PORTS) TLV_T_CONNECT ((TLV_T_PORTID) (TLV_T_SW_NAME) (TLV_T_PORTID))*
*/

#defibe TLV_TTYPE_NONE             0
#define TLV_TTYPE_NOPARAM          1
#define TLV_TTYPE_VALUE            2
#define TLV_TTYPE_DYNAMIC          3

#define TLV_TVALUE_U8              0x80000001
#define TLV_TVALUE_U16             0x80000002
#define TLV_TVALUE_U32             0x80000003
#define TLV_TVALUE_U64             0x80000004

#define TLV_T_TOPOLOGY             0x00000001
#define TLV_T_SW_NAME              0x00000002
#define TLV_T_PORTS                0x00000003
#define TLV_T_CONNECT              0x00000004
#define TLV_T_PORTID               0x00000005

#define IS_TLV_TVALUE(x)           ((x) & 0x80000000)

struct tlv_object {
	int index, length;
	char *buffer;
};

extern int make_tlv(int length);
extern int push_tlv_object(struct tlv_object *obj, int type, int len, char *data);
extern int trim_tlv_object(struct tlv_object *obj);
extern char *get_tlv_buffer(struct tlv_object *obj);
extern void delete_tlv_object(struct tlv_object *obj);

#endif /* TLV_H */
