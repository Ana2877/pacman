#include "library.h"

int main ()
{
    char matriz_lab [LINHA_LAB][COLUNA_LAB];   //MAPA DO JOGO
    int direcao, direcaoT, direcaoAnt;   //DIRE��ES PARA A FUN��O DO PACMAN
    int inicia_jogo; // TECLA NECESS�RIA PARA QUE SE INICIE O JOGO
    FANTASMA fantasma[NUM_FANTASMA]; //ESTRUTURA DOS FANTASMAS
    PACMAN jogador; //ESTRUTURA DO JOGADOR
    int bolachas_normais, bolachas_especiais; // TOTAL DE CADA BOLACHA DURANTE O JOGO
    int i; //VARI�VEL AUXILIAR
    COORDENADA pos_iniciais_fantasmas [NUM_FANTASMA]; // COORDENADAS INICIAIS DOS FANTASMAS
    int rep_fant; //VARIAVEL AUXILIAR PARA A CONTAGEM DE REPETI��ES DA FUN��O DO FANTASMA EM RELA��O A DO PACMAN ENQUANTO O PODER ESTA ATIVADO
    char resposta; // RESPOSTA DO JOGADOR NO FINAL DO JOGO SE DESEJA CONTINUAR OU N�O

    do
    {
        system("cls");
        jogador.vidas = 2; // VIDA INICIAL DO JOGADOR
        jogador.score = 0; // SCORE INICIAL DO JOGADOR
        jogador.poder = 0; // STATUS DO PODE INICIAL DO JOGADOR

        /*
        Seto a posi��o inicial dos fantasmas para uma dire��o qualquer,
        para que somente mude depois que for encontrado um obstaculo.
        Se a primeira dada for um obstaculo j� ocorre a mudan�a.
        */

        for(i = 0; i <NUM_FANTASMA; i ++)
        {
            fantasma[i].dir_fant = 3;
        }

        SetConsoleSize(120, 40);

        desenha_menu();
        entrada_jogo();//CARREGA A TELA DE START

        do
        {
            inicia_jogo = getch(); //APENAS INICIANDO O JOGO QUANDO APERTADA A TECLA ENTER
        }
        while(inicia_jogo != 13);

        if(inicia_jogo == 13)
        {

            system("cls"); // LIMPA A TELA PARA TIRAR A ENTRADA
            le_labirinto(matriz_lab); //RECEBE O LABIRINTO E SALVA NA MATRIZ
            bolachas_normais = conta_bolachas_normais(matriz_lab); //RECEBE O TOTAL DE BOLACHAS NORMAIS
            bolachas_especiais = conta_bolachas_especiais(matriz_lab); //RECEBE O TOTAL DE BOLACHAS ESPECIAIS
            printa_labirinto(matriz_lab); //PRINTA O LABIRINTO NA TELA
            gerador_fantasma(fantasma, matriz_lab, pos_iniciais_fantasmas); // SETA A POSI��O DOS FANTASMAS E OS PRINTA NA TELA
            posicao_pacman(&jogador, matriz_lab); //SETA A POSI��O DO PACMAN E O PRNTA NA TELA
            desenha_menu();
            gotoxy(32, 1);
            textbackground(BLUE);
            printf("%d", jogador.vidas); // J� P�E O TOTAL DE VIDAS NAS BORDAS DO MENU
            gotoxy(12, 1);
            printf("%d", jogador.score); // E O SCORE INCIAL
            textbackground(BLACK);

            do
            {
                rep_fant = 0;

                if(jogador.poder == 1) //ENTRA NESTA PARTE DA FUN��O APENAS QUANDO O PODER ESTA ATIVADO
                {
                    clock_t start, end = 0; //FAZ A CONTAGEM DE TEMPO DE DURA��O DO PODER DO PACMAN AP�S COMER A BOLACHA

                    start = clock (); //SALVA EM QUANTO ESTAVA O CLOCK NA HORA EM QUE PEGOU A BOLACHA

                    do
                    {
                        if(rep_fant == 2) // FAZ COM QUE O FANTASMA SE MOVIMENTE UMA A CADA TRES INTERA��ES
                        {
                            movimenta_todos_fastasmas (fantasma, matriz_lab, &jogador); //FAZ TODO O MOVIMENTO DOS FANTASMAS
                            testa_se_pacman_comeu_fantasma(&jogador, fantasma, matriz_lab, pos_iniciais_fantasmas);
                            rep_fant = 0; //QUANDO MOVE, VOLTA PARA O ZERO E RECOME�A A CONTAGEM
                        }

                        rep_fant ++;

                        if(kbhit())
                        {
                            direcaoT = traduz_teclas();
                            if(direcaoT != 9)
                                direcao = direcaoT;
                        }
                        if(direcao == PAUSE) //TESTA SE O JOGADOR PAUSOU O JOGO
                        {
                            gotoxy(20, 18);
                            textbackground(YELLOW);
                            textcolor(BLACK);
                            printf("JOGO PAUSADO, PRESSIONE QUALQUER TECLA DE MOVIMENTACAO PARA CONTINUAR.");
                            textbackground(BLACK);
                            textcolor(WHITE);
                            while(direcao == PAUSE) // FICA EM UM LOOP ESPERANDO UMA NOVA TECLA DE MOVIMENTA��O PARA CONTINUAR O JOGO
                            {
                                if(kbhit())
                                {
                                    direcaoT = traduz_teclas();
                                    if(direcaoT != 9)
                                        direcao = direcaoT;
                                }
                            }

                            printa_labirinto(matriz_lab); // QUANDO FOR CLICACO, PRINTA O LABIRINTO DE NOVO E SEGUE NORMAL
                        }

                        /*
                                O movimento do pacman � feito de forma que nunca pare a menos que encontre uma parede na sua frente,
                                sendo assim � uma fun��o com retorno 1 (que acontece quando o jogador pressionou uma dire��o e nesta dire��o
                                n�o haviam paredes) ou 0 (caso a dire��o n�o foi poss�vel).
                                Para isso ela faz uso de duas variaveis de dire��o, a antiga e a atual. Sempre tentando executar primeiro a fun��o
                                com a atual e vendo se o movimento foi possivel.
                                - CASO EM QUE MOVIMENTO FOI POSSIVEL, retorna 1 para a main e a dire��o atual, vira a antiga, para a proxima intera��o.
                                - CASO EM QUE MOVIMENTO N�O FOI POSSIVEL, retorna 0 e n�o faz o pacman andar, assim executa a fun��o novamente mas agora com a dire��o antiga.
                        */

                        if((move_pacman (fantasma, &jogador, direcao, direcaoAnt, matriz_lab, &bolachas_especiais, &bolachas_normais)) == 1)
                        {
                            direcaoAnt = direcao;
                        }
                        else
                        {
                            move_pacman(fantasma, &jogador, direcaoAnt, direcao, matriz_lab, &bolachas_especiais, &bolachas_normais);
                        }
                        testa_se_pacman_comeu_fantasma(&jogador, fantasma, matriz_lab, pos_iniciais_fantasmas);

                        end = clock (); //SALVA EM QUANTO ESTA O CLOCK AO FINAL DE CADA INTERA��O
                        Sleep (100);
                    }
                    while((end - start < 5000)); //QUANDO O FINAL MENOS O INICIAL FOR MAIOR QUE 5 SEGUNDOS ACABA A FUN��O
                    jogador.poder = 0; // E O PODER VOLTA A FICAR DESATIVADO
                }



                else // FUN��O NORMAL DO JOGO
                {
                    if(kbhit())
                    {
                        direcaoT = traduz_teclas();
                        if(direcaoT != 9)
                            direcao = direcaoT;
                    }

                    if(direcao == PAUSE) //ANTES DE CONTINUAR A EXECU��O DO JOGO TESTA SE O JOGO N�O ESTA PAUSADO
                    {
                        gotoxy(20, 18);
                        textbackground(YELLOW);
                        textcolor(BLACK);
                        printf("JOGO PAUSADO, PRESSIONE QUALQUER TECLA DE MOVIMENTACAO PARA CONTINUAR.");
                        textbackground(BLACK);
                        textcolor(WHITE);
                        while(direcao == PAUSE)
                        {
                            if(kbhit())
                            {
                                direcaoT = traduz_teclas();
                                if(direcaoT != 9)
                                    direcao = direcaoT;
                            }
                        }

                        printa_labirinto(matriz_lab);
                    }

                    else
                    {
                        movimenta_todos_fastasmas (fantasma, matriz_lab, &jogador);
                        testa_se_fantasma_comeu_pacman (&jogador, fantasma, matriz_lab);

                        if((move_pacman (fantasma, &jogador, direcao, direcaoAnt, matriz_lab, &bolachas_especiais, &bolachas_normais)) == 1)
                        {
                            direcaoAnt = direcao;
                        }
                        else
                        {
                            move_pacman(fantasma, &jogador, direcaoAnt, direcao, matriz_lab, &bolachas_especiais, &bolachas_normais);
                        }

                        testa_se_fantasma_comeu_pacman (&jogador, fantasma, matriz_lab); // CADA VEZ QUE RODA A FUN��O DO PACMAN OU A DO FANTASMA, TESTA SE UM N�O ESTA NA POSI��O DO OUTRO
                    }



                    Sleep (100);
                }

            }
            while(jogador.vidas > 0 && (bolachas_especiais > 0 || bolachas_normais > 0)); // BASE DE EXECU��O DO JOGO
            //ENQUANTO A QUANTIDADE DE VIDAS DO JOGADOR FOR MAIOR QUE 0 E TIVER PELO MENOS UMA BOLACHA NO MAPA



        }

        //QUANDO ACABA O JOGO DA OP��O DE JOGAR NOVAMENTE
        system("cls");
        gotoxy(45, 17);
        textbackground(YELLOW);
        textcolor(BLACK);
        printf("DESEJA JOGAR NOVAMENTE?");
        gotoxy(45, 19);
        textbackground(BLACK);
        textcolor(YELLOW);
        printf("S- sim           N - nao");
        do
        {
            resposta = getch();
            resposta = toupper(resposta);
        }
        while(resposta != 'N' && resposta != 'S'); // S� ACEITA 'S' OU 'N'

    }
    while(resposta != 'N'); //SE DIGITAR 'S' O JOGO RECOME�A NORMAL

    //CASO A PESSOA DIGITE 'N', FINALIZA O JOGO
    system("cls");
    gotoxy(50, 17);
    printf("ATE MAIS!");
    gotoxy(35, 35);
    return 0;
}
