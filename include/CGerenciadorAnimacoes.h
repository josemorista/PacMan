#include "CAnimacao.h"
class CGerenciadorAnimacoes{

private:

    static SDL_Renderer *render;
    static int totalAnimacoes;
    static PoolNumeros numAnimacoes;
    static Animacao animacoes[MAX_ANIMACOES];

public:

    static Animacao GetAnimacao(int id_animacao){
        return animacoes[id_animacao];
    }

    static void Inicia(SDL_Renderer *renderer){
        render = renderer;
        totalAnimacoes = 0;
        for (int i=0;i<MAX_ANIMACOES;i++)
            animacoes[i] = NULL;
        numAnimacoes = new CPoolNumeros(MAX_ANIMACOES);
    }

    static void Encerra(){
        for (int i=0;i<MAX_ANIMACOES;i++){
            if (animacoes[i]) delete animacoes[i];
        }
        delete numAnimacoes;
    }

    static int CriaAnimacao(char* nomeArquivoBMP,int retiraFundo=1,int opacidade=255){
        int resp = numAnimacoes->RetiraLivre();
        animacoes[resp] = new CAnimacao(0,0,render,nomeArquivoBMP,retiraFundo,opacidade);
        totalAnimacoes++;
        return resp;
    }

    static int CriaAnimacao(int id_animacaoBase){
        int resp = numAnimacoes->RetiraLivre();
        animacoes[resp] = new CAnimacao(0,0,animacoes[id_animacaoBase]);
        totalAnimacoes++;
        return resp;
    }

    static void DestroiAnimacao(int id_animacao){
        numAnimacoes->DevolveUsado(id_animacao);
        delete animacoes[id_animacao];
        totalAnimacoes--;
        animacoes[id_animacao] = NULL;
    }

    inline static void SetDimensoesAni(int idanimacao,int altura,int largura)
    {
        animacoes[idanimacao]->SetDimensoes(altura,largura);
    }

    inline static void Move(int id_animacao,int px,int py){
        animacoes[id_animacao]->Move(px,py);
    }

    inline static void Desenha(int id_animacao){
        animacoes[id_animacao]->Desenha();
    }

    inline static void CriaFrame(int id_animacao,int codFrame,int x,int y,int altura,int largura){
        animacoes[id_animacao]->CriaFrame(codFrame,x,y,altura,largura);
    }

    inline static void CriaModo(int id_animacao,int idModo, int loop){
        animacoes[id_animacao]->CriaModo(idModo,loop);
    }

    inline static void InsereFrame(int id_animacao, int idModo,int idFrame, float delayFrame){
        animacoes[id_animacao]->InsereFrame(idModo,idFrame,delayFrame);
    }

    inline static void MudaModo(int id_animacao,int idModo,int inicia){
        animacoes[id_animacao]->MudaModo(idModo,inicia);
    }

    inline static void SetOpacidade(int id_animacao,int valor){
        animacoes[id_animacao]->SetOpacidade(valor);
    }

    inline static int ColisaoAnimacoes(int id_animacao1,int id_animacao2){
        return animacoes[id_animacao1]->Colisao(animacoes[id_animacao2]);
    }

    inline static int ColisaoObjeto(int id_animacao,Objeto obj){
        return animacoes[id_animacao]->Colisao(obj);
    }

    inline static void GETXYAni(int id_animacao,int *x,int *y){
        *x=animacoes[id_animacao]->x;
        *y=animacoes[id_animacao]->y;
    }

};

SDL_Renderer* CGerenciadorAnimacoes::render;
PoolNumeros CGerenciadorAnimacoes::numAnimacoes;
int CGerenciadorAnimacoes::totalAnimacoes;
Animacao CGerenciadorAnimacoes::animacoes[MAX_ANIMACOES];

