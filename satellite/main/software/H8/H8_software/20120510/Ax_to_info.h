void Ax_to_info(unsigned char *,int );
void crcbit (unsigned char);/*FCS�v�Z�p�֐�*/
void precrc_bit(unsigned char);/*�P�U�i�����Q�i���ɕϊ�����֐�*/
unsigned char  fcslo, fcshi;/*FCS�v�Z����ϐ�*/
int StrLen(const char *);/*�z��̒������擾����֐�*/
int info_length;/*info�̕�����̒���*/
static unsigned char *info;/*info�̕����񂪂��̕ϐ���ɋL�^�����*/
