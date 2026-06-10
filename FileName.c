#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#define ANSI_RED "\x1b[31m"             //赤文字表示用
#define ANSI_BLUE "\x1b[34m"            //青文字表示用
#define ANSI_RESET "\x1b[0m"            //文字色リセット用
#define min_value 110                   //商品の最低価格(110円)
#define TYPES 5                         //構造体配列の要素数
#define MAX 100                         //文字列用配列の要素数

typedef struct {
    char name[100]; // 商品名 (MAX 10 バイト + null) [4] 
    int price; // 単価 (100 円～300 円) [4] 
    int stock; // 在庫数 (初期値 20) [4] 
} Product;

typedef struct {
    char type_name[100]; // 金種名 ("100 円" など) 
    int value; // 金額 (10, 50, 100, 500, 1000) 
    int count; // 保持枚数 
    int inserted; // 今回の投入枚数 (上限チェック用) [6] 
} Money;

Product Products[TYPES];
Money Moneys[TYPES];
int all_Count = 0;
int length = 5;

//ファイルの内容を読み込む関数
gg Gin san tanakasan
void load_data()
{
    FILE* p_Fp = fopen("items.csv", "r");
    FILE* m_Fp = fopen("money.csv", "r");

    if (p_Fp == NULL || m_Fp == NULL)               //ファイルエラー判定
    {
        printf("ファイルが開けませんでした\n");
        exit(0);
    }
    for (all_Count;all_Count < TYPES; all_Count++) {

        if (all_Count < MAX &&                       //ファイルの内容を読み込む処理
            fscanf(p_Fp, "\"%49[^\"]\",%d,%d\n",
                Products[all_Count].name,
                &Products[all_Count].price,
                &Products[all_Count].stock) == 3 &&
            fscanf(m_Fp, "\"%49[^\"]\",%d,%d\n",
                Moneys[all_Count].type_name,
                &Moneys[all_Count].value,
                &Moneys[all_Count].count) == 3)
        {

            printf("%d\n",all_Count);
            Moneys[all_Count].inserted = 0;

            if (Products[all_Count].price > 300 || Products[all_Count].price < 100) {
                printf("商品の価格は100～300円の間にしてください。\n");

                fclose(p_Fp);
                fclose(m_Fp);

                exit(0);
            }
        }
        else {
            printf("CSVファイルを確認してください！！！");
            fclose(p_Fp);
            fclose(m_Fp);
            exit(0);
        }
    
    }

    fclose(p_Fp);
    fclose(m_Fp);

}

//ファイルにデータを書き込む関数
void save_data()
{
    FILE* p_Fp = fopen("items.csv", "w");
    FILE* m_Fp = fopen("money.csv", "w");

    if (p_Fp == NULL || m_Fp == NULL)           //ファイルエラー判定
    {
        printf("File open error\n");
        exit(0);
    }

    for (int i = 0; i < length; i++)            //ファイルにデータを書き込む処理
    {
        fprintf(p_Fp, "\"%s\",%d,%d\n",
            Products[i].name,
            Products[i].price,
            Products[i].stock);
        fprintf(m_Fp, "\"%s\",%d,%d\n",
            Moneys[i].type_name,
            Moneys[i].value,
            Moneys[i].count);
    }

    fclose(p_Fp);
    fclose(m_Fp);
}

//メニュー表示関数
void display_menu(int purchase) {

    int total = 0;      //入金の合計額

    int i;              //ループカウンタ

    char mark = 'A';    //ABC表示のための最初の文字

    //合計入金額計算処理
    for (i = 0;i < TYPES;i++) {
        total += Moneys[i].value * Moneys[i].inserted;
    }

    printf(" *********** 自動販売機 シミュレーション ソフトウェア ********** \n\n");


    printf("金額投入後商品を選択してください。");

    if (purchase != -1) {           //商品が購入されていた場合
        printf(ANSI_BLUE"	%sの購入、ありがとうございました。"ANSI_RESET, Products[purchase].name);
    }
    else if (total > 0) {           //入金額がある場合
        printf(ANSI_BLUE"	入金%d円"ANSI_RESET, total);
    }

    printf("");

    printf("\n");

    printf("+--------------+--------------+--------------+--------------+--------------+\n");

    for (i = 0;i < TYPES;i++) {         //商品名
        printf("|  %c.%-8s  ", mark + i, Products[i].name);
    }

    printf("|\n+--------------+--------------+--------------+--------------+--------------+\n");

    for (i = 0;i < TYPES;i++) {         //金額表示

        if (Products[i].stock == 0) {                       //売り切れ表示
            printf("|"ANSI_RED"     売切     "ANSI_RESET);
        }
        else if (total >= Products[i].price) {              //価格を青文字で表示
            printf("|"ANSI_BLUE"     %3d      "ANSI_RESET, Products[i].price);
        }
        else {                                              //価格を表示
            printf("|     %3d      ", Products[i].price);
        }


    }

    printf("|\n+--------------+--------------+--------------+--------------+--------------+\n");

    if (purchase != -1) {       //商品が購入されていた場合
        printf(ANSI_BLUE"お釣りは%d円です。%sを受け取りください。\n"ANSI_RESET, total, Products[purchase].name);
    }


}

//入金処理関数
int process_payment(int m_unit) {


    if (m_unit == 4) {                  //1000円の場合
        if (Moneys[m_unit].inserted + 1 > 2) {  //1000円が2枚を超える場合
            return m_unit;                      //エラー判定で添え字を返す
        }
        else {                                  //超えない場合入金
            Moneys[m_unit].inserted++;
            return -1;                          //成功判定を返す
        }
    }
    else {                              //1000円以外の場合
        if (Moneys[m_unit].inserted + 1 > 20) { //硬貨が20枚を超える場合
            return m_unit;                      //エラー判定で添え字を返す
        }
        else {                                  //超えない場合入金
            Moneys[m_unit].inserted++;
            return -1;                          //成功判定を返す
        }
    }

}

//商品購入処理関数
int select_product(int drink) {

    int total = 0;      //合計用
    int i;              //ループカウンタ
    int flug;           //購入用フラグ

    //合計入金額計算
    for (i = 0;i < TYPES;i++) {
        total += Moneys[i].value * Moneys[i].inserted;
    }

    if (Products[drink].price > total) {        //お金が足りない場合
        return 5;
    }

    if (Products[drink].stock == 0) {           //在庫がない場合
        return 6;
    }

    flug = dispence_change(drink);              //お釣りの計算関数

    if (flug == -1) {                           //問題なかった場合ドリンクを減らしてファイルに記録
        Products[drink].stock--;

        save_data();

        return -1;
    }
    else {                                      //お釣りが出せなかった場合足りない硬貨の添え字を返す
        return flug;
    }



}

//お釣り計算関数
int dispence_change(int drink) {

    int total = 0;          //合計用
    int i;                  //ループカウンタ
    int flug = -1;          //お釣り払い出し用フラグ
    int o_change;           //お釣りの金額記録用
    int change[TYPES];      //お釣りの金種記録用

    //合計入金額計算
    for (i = 0;i < TYPES;i++) {
        total += Moneys[i].value * Moneys[i].inserted;
        change[i] = 0;
    }

    o_change = total - Products[drink].price;       //お釣りの金額を計算

    for (i = 4;i >= 0;i--) {                         //5回ループ

        if (o_change == 0) {                        //お釣りがゼロになったら抜ける
            break;
        }

        while (o_change >= Moneys[i].value) {       //お釣りがその金種より大きい間ループ


            if (Moneys[i].count > 0) {              //その金種の在庫がある場合お釣りの金種に記録
                change[i]++;
                o_change -= Moneys[i].value;
            }
            else {
                flug = i;                           //在庫がない場合金種を記録する
                break;
            }
        }


    }

    if (o_change == 0) {                            //お釣りの残金が0だった場合
        for (i = 0;i < TYPES;i++) {                 //お釣り分のお金を金種の在庫から引く

            Moneys[i].count += Moneys[i].inserted;
            Moneys[i].count -= change[i];
            Moneys[i].inserted = change[i];
        }

        return -1;                                  //成功判定を返す
    }
    else {
        return flug;                                //お釣りが返せなかった場合足りない金種を返す
    }
}


int main(void)
{
    int i = 0;                  //ループカウンタ
    int products = -1;          //商品購入判定
    char user_Input[10];        //ユーザーの入力用配列
    int total = 0;              //合計入金額計算用
    int flug = 0;               //ユーザーの入力が有効かを判断するフラグ
    int s_flug = -2;            //商品購入ができたかどうかの判別フラグ
    int p_flug = -1;            //入金ができたかどうかの判別フラグ
    int Input = -1;             //購入ができた際に買った商品を記録する変数
    int buy_flug = 0;           //商品購入表示用フラグ
    int finish = 0;             //終了判定フラグ


    load_data();       //ファイル読み込み

    while (1)
    {
        total = 0;                                              //合計入金額初期化

        //system("cls");											//画面を初期化する処理

        for (i = 0;i < TYPES;i++) {                             //合計入金額計算
            total += Moneys[i].value * Moneys[i].inserted;
        }

        display_menu(products);                                 //メニュー表示

        do {

            flug = 0;                                           //ユーザー入力用フラグ初期化


            printf("お金を入れてください。");

            for (i = 0; i < length; i++)
            {
                printf("%d:%s ", i + 1, Moneys[i].type_name);
            }

            if (total >= min_value) {                           //入金額が最低価格以上の場合
                printf("\n商品購入可能です。");
            }

            if (p_flug != -1) {                                 //入金処理でエラーが起きた場合
                if (p_flug == 4) {           //1000円の場合
                    printf(ANSI_RED"\n%s紙幣の投入枚数が上限(2枚)を超えています。"ANSI_RESET, Moneys[p_flug].type_name);
                }
                else {                        //1～500円の場合
                    printf(ANSI_RED"\n%s枚数の投入枚数が上限(20枚)を超えています。"ANSI_RESET, Moneys[p_flug].type_name);
                }
            }
            else if (s_flug != -1 && s_flug != -2) {            //商品購入処理でエラーが起きた場合
                if (s_flug == 5) {                              //金額不足の場合
                    printf(ANSI_RED"\n投入金額が足りていません。"ANSI_RESET);

                }
                else if (s_flug == 6) {                         //在庫不足の場合
                    printf(ANSI_RED"\n売り切れ商品です。他の商品を選択してください。"ANSI_RESET);
                }
                else if (s_flug == 4) {                           //1000円の釣銭不足の場合
                    printf(ANSI_RED"\n%s紙幣の釣銭切れのため商品を購入できません。"ANSI_RESET, Moneys[s_flug].type_name);
                }
                else {                                          //それ以外の硬貨の釣銭不足の場合

                    printf(ANSI_RED"\n%s硬貨の釣銭切れのため商品を購入できません。"ANSI_RESET, Moneys[s_flug].type_name);
                }
            }

            s_flug = -2;                                        //商品購入用フラグの初期化

            if (products != -1) {                               //商品が購入されていた場合
                buy_flug = 1;                                   //商品購入用フラグを真にする
                break;
            }

            if (finish) {                                       //終了コード(9)が入力されていた場合

                if (total > 0) {                                //入金額があった場合返金処理

                    printf(ANSI_BLUE"\n%d円を返金します。お受け取りください。\n"ANSI_RESET, total);

                    for (i = 0;i < TYPES;i++) {
                        Moneys[i].inserted = 0;
                    }
                }

                save_data();                                    //ファイルに記録

                Sleep(10000);                                   //10秒待機

                return 0;
            }

            printf("\n\n>> ");



            scanf(" %s",&user_Input);
            while (getchar() != '\n');                          //標準入力初期化

            if (strcmp(user_Input, "9") == 0) {                 //終了コード(9)が入力された場合終了判定フラグを真にする
                finish = 1;
                break;
            }

            for (i = 0;i < 5;i++) {                             //1～5の入力判定
                if ((user_Input[0] - '0') == (1 + i)) {
                    // 一致したときの処理
                    flug = 1;
                    Input = i;
                    break;
                }
            }

            if (flug == 1) {                                    //0～5だった場合入金処理
                p_flug = process_payment(Input);
            }
            else {
                for (i = 0;i < 5;i++) {                         //A～Eの入力判定
                    if ((user_Input[0] - 'A') == i) {
                        // 一致したときの処理
                        flug = 1;
                        Input = i;
                        break;
                    }
                }

                if (flug == 1) {                                //A～Eだった場合購入処理
                    s_flug = select_product(Input);
                }
            }

            if (flug != 1) {                                    //それ以外の場合エラー文を表示してループを戻る
                printf("無効な入力です。\n");
                continue;
            }

        } while (flug == 0);                                    //入力が0以外の場合ループを抜ける


        if (s_flug == -1) {                                     //商品購入が行われた場合買った商品の添え字を格納
            products = Input;
        }

        if (buy_flug) {                                         //商品購入表示フラグが真だった場合
            buy_flug = 0;                                       //商品購入フラグを初期化
            products = -1;                                      //購入商品記録変数の初期化

            if (total < min_value) {                            //お釣りの額が最低金額より下の場合
                for (i = 0;i < TYPES;i++) {                     //入金額初期化(払い出し)処理
                    Moneys[i].inserted = 0;
                }
            }

            Sleep(10000);                                       //10秒待機
            continue;
        }

        //たぬき
    }

}