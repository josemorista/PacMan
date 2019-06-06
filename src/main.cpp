#include "PIG.h"
#include "time.h"
#include "stdlib.h"


PIG_Evento evento;
PIG_Teclado meuTeclado;

typedef struct fantasma{
    int preso;
    int x,y;
    int animacao;
}Fantasma;

typedef struct posicao
{
    int item;
    int valido;
}Campo;

Campo campo[15][13];
int timermovimentos,timerbolado,timerbrilho,timerfantasmas,timermorto;
int desenhamorto=0,pressionada=0,pontos=0;
int pacman,px=0,py=0,pacmanbolado=0;
int numcomidas=0,morto;
Fantasma fantasmas[4];

int moverFantasma(int n,int dir)
{
    int i,j,fx,fy;
    getXYAnimacao(fantasmas[n].animacao,&fx,&fy);

    i=14-((fy/40)%15);
    j=(fx/40)%13;


    if(dir==0)
    {
        if((j+1<=12) && campo[i][(j+1)].valido==1){
            MoveAnimacao(fantasmas[n].animacao,fx+40,fy);
            return 1;
        }
        return 0;

    }else if(dir==1)
    {
        if((j-1>=0) && (campo[i][(j-1)].valido==1)){
            MoveAnimacao(fantasmas[n].animacao,fx-40,fy);
            return 1;
        }
        return 0;
    }else if(dir==2)
    {
        if((i-1<=14) && (campo[i-1][j].valido==1) || fantasmas[n].preso>0){
            MoveAnimacao(fantasmas[n].animacao,fx,fy+40);
            return 1;
        }
        return 0;
    }else
    {
        if((i+1>=0) && (campo[i+1][j].valido==1)){
                MoveAnimacao(fantasmas[n].animacao,fx,fy-40);
                return 1;
        }
        return 0;
    }

}

void moverFantasmas()
{
    srand(unsigned(time(NULL)));
    if(TempoDecorrido(timerfantasmas)>0.25)
    {
        int i,j,cont;
        for(cont=0;cont<4;cont++)
        {
            if(fantasmas[cont].preso==0){
                int aleat;
                aleat=rand() % 4;
                moverFantasma(cont,aleat);
            }else
            {
                fantasmas[cont].preso++;
                if(fantasmas[cont].preso>12-cont)
                {
                    fantasmas[cont].preso=0;
                }
            }
        }
        ReiniciaTimer(timerfantasmas);
    }

}

int NovoFantasma(char *nomearquivo)
{
    int animacao = CriaAnimacao(nomearquivo,0,255);
    int i,j;
    CriaModoAnimacao(animacao,0,1);
    for(i=0;i<8;i++)
    {
        CriaFrameAnimacao(animacao,i,i*32,0,31,35);
        if(i>0)
        InsereFrameAnimacao(animacao,0,i,0.5);
        else
        InsereFrameAnimacao(animacao,0,i,0.02);
    }
    SetDimensoesAnimacao(animacao,38,38);
    MudaModoAnimacao(animacao,0,1);
    return animacao;
}

int CriarPacman(char* nomearquivo)
{
    int animacao = CriaAnimacao(nomearquivo,0,255);
    int i=0,j;
    for(i=0;i<4;i++){
        CriaModoAnimacao(animacao,i,1);
        for(j=0;j<2;j++)
        {
            CriaFrameAnimacao(animacao,i*4+j,j*32+(i*64),0,31,35);
            InsereFrameAnimacao(animacao,i,i*4+j,0.06);
        }
    }
    SetDimensoesAnimacao(animacao,40,40);
    return animacao;
}

void inicializaCampo()
{
    int i,j;
    FILE *arq=fopen("campo.txt","rt");
    for(i=0;i<15;i++)
    {
        for(j=0;j<13;j++)
        {
            fscanf(arq,"%d %d\n",&campo[i][j].valido,&campo[i][j].item);
        }
        fscanf(arq,"\n");
    }
    fclose(arq);
}
void mudaModoPacman()
{
    if(evento.tipoEvento==EVENTO_TECLADO)
    {
        pressionada=1;
        if(evento.teclado.tecla==TECLA_DIREITA)
        {
            if(pressionada==1)
            {
                MudaModoAnimacao(pacman,0,0);
                pressionada=0;
            }
        }
        if(evento.teclado.tecla==TECLA_ESQUERDA)
        {
            if(pressionada==1)
            {
                MudaModoAnimacao(pacman,2,0);
                pressionada=0;
            }
        }
        if(evento.teclado.tecla==TECLA_CIMA)
        {
            if(pressionada==1)
            {
                MudaModoAnimacao(pacman,3,0);
                pressionada=0;
            }
        }
        if(evento.teclado.tecla==TECLA_BAIXO)
        {
            if(pressionada==1)
            {
                MudaModoAnimacao(pacman,1,0);
                pressionada=0;
            }
        }

    }

}
void moverPacman()
{
    int i,j;
    if(evento.tipoEvento==EVENTO_TECLADO)
    {
        if(evento.teclado.acao==TECLA_PRESSIONADA && TempoDecorrido(timermovimentos)>0.035)
        {
            i=14-((py/40)%15);
            j=(px/40)%13;

            if(evento.teclado.tecla==TECLA_DIREITA)
            {
                if((j+1<=12) && campo[i][(j+1)].valido==1){
                    MoveAnimacao(pacman,px+=40,py);
                    j++;
                    if(campo[i][j].item==2)
                    {
                        pacmanbolado=1;
                        pontos+=90;
                        ReiniciaTimer(timerbolado);
                    }

                    if(campo[i][j].item!=0){
                        pontos+=10;
                        numcomidas--;
                    }

                    campo[i][j].item=0;



                }

            }
            if(evento.teclado.tecla==TECLA_ESQUERDA)
            {
                if((j-1>=0) && (campo[i][(j-1)].valido==1)){
                    MoveAnimacao(pacman,px-=40,py);
                    j--;
                     if(campo[i][j].item==2)
                    {
                        pacmanbolado=1;
                        pontos+=90;
                        ReiniciaTimer(timerbolado);
                    }

                    if(campo[i][j].item!=0){
                        pontos+=10;
                        numcomidas--;
                    }

                    campo[i][j].item=0;


                }
            }
            if(evento.teclado.tecla==TECLA_CIMA)
            {
                if((i-1<=14) && (campo[i-1][j].valido==1)){
                    MoveAnimacao(pacman,px,py+=40);
                    i--;
                     if(campo[i][j].item==2)
                    {
                        pacmanbolado=1;
                        pontos+=90;
                        ReiniciaTimer(timerbolado);
                    }

                    if(campo[i][j].item!=0){
                        pontos+=10;
                        numcomidas--;
                    }

                    campo[i][j].item=0;


                }
            }
            if(evento.teclado.tecla==TECLA_BAIXO)
            {
                if((i+1>=0) && (campo[i+1][j].valido==1)){
                    MoveAnimacao(pacman,px,py-=40);
                    i++;
                     if(campo[i][j].item==2)
                    {
                        pacmanbolado=1;
                        pontos+=90;
                        ReiniciaTimer(timerbolado);
                    }

                    if(campo[i][j].item!=0){
                        pontos+=10;
                        numcomidas--;
                    }
                    campo[i][j].item=0;


                }
            }
        }
        ReiniciaTimer(timermovimentos);
    }
}

int TestaMortePacman()
{
    int i;
    for(i=0;i<4;i++)
    {
        if(ColisaoAnimacoes(pacman,fantasmas[i].animacao)==1)
        {
            if(pacmanbolado==1)
            {
                int xg,yg;
                getXYAnimacao(fantasmas[i].animacao,&xg,&yg);
                MoveAnimacao(morto,xg,yg);
                MoveAnimacao(fantasmas[i].animacao,202+40*i,282);
                desenhamorto=1;
                ReiniciaTimer(timermorto);
                fantasmas[i].preso=1;
            }else
            {
                return 1;
            }
        }
    }
    return 0;
}

void Preparacampo()
{
    int i;
    for(i=0;i<4;i++)
    {
        fantasmas[i].x=202+40*i;
        fantasmas[i].x=362;
        fantasmas[i].preso=1;
        MoveAnimacao(fantasmas[i].animacao,202+40*i,282);
    }
    numcomidas=(13*15)-79;
    px=240;
    py=360;
    MoveAnimacao(pacman,px,py);
}

void DesenhaComidas(int comida,int fruta)
{
    int i,j;
    for(i=0;i<15;i++)
    {
        for(j=0;j<13;j++)
        {
            if(campo[i][j].item==1)
            {
                MoveObjeto(comida,(40*j)+15,40*(14-i)+15);
                DesenhaObjeto(comida);
            }else if(campo[i][j].item==2)
            {
                MoveObjeto(fruta,(40*j)+10,40*(14-i)+10);
                DesenhaObjeto(fruta);
            }
        }
    }
}
int main( int argc, char* args[] )
{
    CriaJogo("Meu Jogo",0);
    meuTeclado = GetTeclado();
    int i,win,vida,gameover,menujogar,inicio=0,vidas=3,fundo=CriaObjeto("..//imagens//fundo.png",0);
    int fruta,comida=CriaObjeto("..//imagens//comida.png");
    fruta=CriaObjeto("..//imagens//comidamaior.png",0);
    menujogar=CriaObjeto("..//imagens//menu.png",0);
    vida=CriaObjeto("..//imagens//vida.png",0);
    gameover=CriaObjeto("..//imagens//gameover.png",0);
    win=CriaObjeto("..//imagens//win.png",0);

    SetEstadoJogo(2);

    timermovimentos=CriaTimer();
    timerbolado=CriaTimer();
    timerfantasmas=CriaTimer();
    timerbrilho=CriaTimer();
    timermorto=CriaTimer();

    morto=NovoFantasma("..//imagens//morto.png");
    fantasmas[0].animacao=NovoFantasma("..//imagens//redghost.png");
    fantasmas[1].animacao=NovoFantasma("..//imagens//yellowghost.png");
    fantasmas[2].animacao=NovoFantasma("..//imagens//yellowghost.png");
    fantasmas[3].animacao=NovoFantasma("..//imagens//redghost.png");
    pacman = CriarPacman("..//imagens//pacman.png");

    while(JogoRodando()){
        evento = GetEvento();


        IniciaDesenho();

        if(GetEstadoJogo()==4)
        {
            char aux[20];
            DesenhaObjeto(win);
            sprintf(aux,"Pontos: %d",pontos);
            EscreverEsquerda(aux,320,160);
            if(evento.tipoEvento==EVENTO_TECLADO)
            {
                if(evento.teclado.tecla==TECLA_ENTER)
                {
                    pontos=0;
                    SetEstadoJogo(2);
                }
            }
        }

        if(GetEstadoJogo()==3)
        {
            char aux[20];
            DesenhaObjeto(gameover);
            sprintf(aux,"Pontos: %d",pontos);
            EscreverEsquerda(aux,320,160);
            if(evento.tipoEvento==EVENTO_TECLADO)
            {
                if(evento.teclado.tecla==TECLA_ENTER)
                {
                    pontos=0;
                    SetEstadoJogo(2);
                }
            }
        }

        if(GetEstadoJogo()==2)
        {
            DesenhaObjeto(menujogar);

            if(evento.tipoEvento==EVENTO_MOUSE && evento.mouse.acao==MOUSE_PRESSIONADO)
            {
                if(evento.mouse.botao==MOUSE_ESQUERDO)
                {
                    int x,y;
                    x=evento.mouse.posX;
                    y=evento.mouse.posY;
                    if(x>=300 && x<=500 && y>=120 && y<=180)
                    {
                        SetEstadoJogo(0);
                    }
                }
            }
        }

        if(GetEstadoJogo()==0)
        {
            inicializaCampo();
            Preparacampo();
            inicio=1;
            SetEstadoJogo(1);//Jogo executando state!
        }

        if(GetEstadoJogo()==1)
        {
             if(TestaMortePacman()==1)
            {
                vidas--;
                if(vidas>0){
                   SetEstadoJogo(0);//Prepara jogo state!
                }else
                {
                    SetEstadoJogo(3);//Gamer Over state!
                    vidas=3;
                }
            }

            if(numcomidas==0)
            {
                SetEstadoJogo(4);//winning state!
            }

            DesenhaObjeto(fundo);
            DesenhaComidas(comida,fruta);

            if(desenhamorto==1)
            {
                DesenhaAnimacao(morto);
                if(TempoDecorrido(timermorto)>0.2)
                {
                    desenhamorto=0;
                }

            }
            char aux[20];
            sprintf(aux,"%d",pontos);
            EscreverEsquerda(aux,580,500);

            mudaModoPacman();

            moverFantasmas();

            for(i=0;i<vidas;i++)
            {
                MoveObjeto(vida,560+i*45,400);
                DesenhaObjeto(vida);
            }

            for(i=0;i<4;i++)
            {
                DesenhaAnimacao(fantasmas[i].animacao);
            }

            if(pacmanbolado==1)
            {

                if(TempoDecorrido(timerbrilho)>0.2)
                {
                    SetOpacidadeAnimacao(pacman,150);
                }

                if(TempoDecorrido(timerbrilho)>0.4)
                {
                    SetOpacidadeAnimacao(pacman,255);
                    ReiniciaTimer(timerbrilho);
                }
                if(TempoDecorrido(timerbolado)>5)
                {
                    pacmanbolado=0;
                    SetOpacidadeAnimacao(pacman,255);
                }
            }
            DesenhaAnimacao(pacman);
            moverPacman();
        }

        EncerraDesenho();
    }
    FinalizaJogo();
    return 0;
}
