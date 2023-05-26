typedef struct
{
    String name;
    String stup;
    String pr1;
    String pr2;
} Stp;
void SetUpStp(String *cfg, Stp *ar_stp);
void genGetStatus(Stp bmk_num);
void genGetPr(Stp bmk_num);
int genAnswer(String com, Stp *arr_stp);
// void calcNextCar(Stp *arr_stp);
String readCom();