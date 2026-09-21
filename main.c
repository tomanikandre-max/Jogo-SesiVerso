#include <genesis.h>
#include "graficos.h"
#define WALK 0
#define WALK1 1
#define JUMP 2
#define PARADO 3

#define barata 0
#define barata2 1

#define LataDeLixo 0

#define Lixo 1

typedef struct {
    int x, y, largura, altura;
} Hitbox;


typedef struct
{
    Hitbox Hitbox;
    int contador;
} menos_lixo;

typedef struct {
    Sprite* sprite;
    int x;
    int y;
    int estado;
    int c_animacao;
    int c_impulso_pulo;
    int c_trava_recarga;
    int c_topo_parabola;
    int c_suave_subida;
    int c_suave_queda;
    bool alternar_passo;
    bool esta_caindo_suave;
    bool fase_queda_rapida;
    bool pode_pular;
    Hitbox Hitbox;
} PlayerEntidade;

typedef struct{

    Sprite* sprite;
    int x;
    int y;
    int c_animacao;
    bool alterar_passo;
    Hitbox Hitbox;
    bool modo;
    int estado;
    int vel;

} Barata;

typedef struct{

    Sprite* sprite;
    int lixos;
    Hitbox Hitbox;

} lata;

typedef struct{

    Sprite* sprite;
    int x, y;
    Hitbox Hitbox;

} lixo;

void drawTextCentered(const char* str, u16 y)
{
    u16 x = (40 - strlen(str)) / 2;
    VDP_drawText(str, x, y);
}

u8 checar_colisao(Hitbox box1, Hitbox box2) {
    if (box1.x < box2.x + box2.largura &&
        box1.x + box1.largura > box2.x &&
        box1.y < box2.y + box2.altura &&
        box1.y + box1.altura > box2.y) {
        return 1;
    }
    return 0;
}

int main()
{
    VDP_waitVSync();
    XGM_setPCM(65, som_pulo, sizeof(som_pulo));
    XGM_startPlay(musica_fundo);
    // pong
    int ea = 0;
    int vel2=0;
    int mode = false;
    int pontose=0;
    int pontosd=0;
    char texto[90] = " ";
    sprintf(texto, "%d     |     %d", pontose, pontosd);
    u16 altura  = VDP_getScreenHeight();
    SPR_init();
    PAL_setPalette(PAL0, player_sprite.palette->data, DMA);
    Sprite* d;
    Sprite* e;
    Sprite* i;
    int d_y = 150;
    int e_y = 150;
    float i_x = 150.0;
    int i_y = 150;
    int estado = 0;
    float vel = 3.0;
    Hitbox d_box;
    d_box.largura = 16;
    d_box.altura = 16;
    d_box.x = 300;
    Hitbox e_box;
    e_box.x = 0;
    e_box.largura = 16;
    e_box.altura = 16;
    Hitbox i_box;
    i_box.x = 100;
    i_box.largura = 8;
    i_box.altura = 8;

    // plataforma
    int fase_em_que_esta = 0;
    int cam_x = 0;
    int limite = 400;
    Barata Barata;
    PlayerEntidade player;
    player.x = 30;
    player.y = 150;
    player.estado = 0;
    player.c_animacao = 0;
    player.c_impulso_pulo = 0;
    player.c_trava_recarga = 0;
    player.c_topo_parabola = 0;
    player.c_suave_subida = 0;
    player.c_suave_queda = 0;
    player.alternar_passo = false;
    player.esta_caindo_suave = false;
    player.fase_queda_rapida = false;
    player.pode_pular = true;
    player.Hitbox.x = 30;
    player.Hitbox.y = 150;
    player.Hitbox.largura = 16;
    player.Hitbox.altura = 16;

    Barata.modo = false;
    Barata.c_animacao = 0;
    Barata.Hitbox.x = 300;
    Barata.Hitbox.y = 150;
    Barata.x = 280;
    Barata.y = 150;
    Barata.alterar_passo=false;
    Barata.Hitbox.largura = 16;
    Barata.Hitbox.altura = 16;
    Barata.estado = 0;
    Barata.vel = 0;

    int mode2 = 0;
    bool segurando  = false;
    int lixos_necessarios = 3;
    bool modo = false;
    int contador = 0;
    bool pular = false;
    int mortes = 0;
    bool menos_lixos_bool = false;
    char Lixo1[40] = " ";
    char morte[40] = " ";
    sprintf(morte, "Mortes: %d", mortes);
    u16 x = 15 - (strlen(mortes) / 2);
    u16 y = 1;
    u16 y2 = 2;
    lata Lata;
    lixo lixo;
    int limite_cam = 320;
    menos_lixo menos_lixo;
    menos_lixo.contador=0;

    Hitbox Agua1;
    Hitbox Agua2;

    bool mudar = true;

    while(1)
    {
        u16 joy = JOY_readJoypad(JOY_1);
    
        if(fase_em_que_esta==0)
        {
        if(mudar)
        {

        VDP_drawImage(BG_B, &nuvem, 70, 10);
        VDP_drawImage(BG_B, &nuvem, 90, 2);
        VDP_drawImage(BG_B, &chao_imagem, 0, 20);
        VDP_drawImage(BG_B, &chao_imagem, 80, 20);
        VDP_drawImage(BG_B, &chao_imagem, 300, 20);
        VDP_drawImage(BG_B, &agua, 40, 20);
        VDP_drawImage(BG_B, &agua, 20, 20);
        SPR_init();
        PAL_setPalette(PAL0, player_sprite.palette->data, DMA);
        player.sprite = SPR_addSprite(&player_sprite, player.x, player.y, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        Barata.sprite = SPR_addSprite(&barata_sprite, Barata.x, Barata.y, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        Lata.sprite = SPR_addSprite(&lata_sprite,  400, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        lixo.sprite = SPR_addSprite(&lixo_sprite, 25, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));

        Lata.lixos = 0;
        Lata.Hitbox.x = 400;
        Lata.Hitbox.y = 150;
        Lata.Hitbox.largura = 16;
        Lata.Hitbox.altura = 16;
        lixo.Hitbox.x = 25;
        lixo.Hitbox.y = 150;
        lixo.Hitbox.largura = 16;
        lixo.Hitbox.altura = 16;
        lixo.x = 25;
        lixo.y = 150;

        Agua1.x = 240; 
        Agua1.y = 160;
        Agua1.largura = 32;
        Agua1.altura = 32;

        Agua2.x = 160; 
        Agua2.y = 160;
        Agua2.largura = 32;
        Agua2.altura = 32;

        SPR_setFrame(Lata.sprite, LataDeLixo);
        SPR_setFrame(lixo.sprite, Lixo);
        SPR_setHFlip(Barata.sprite, TRUE);
        mudar = false;
        }
        Barata.vel = (random() % 1) + 3;
        mode2 = 0;
        lixos_necessarios = 2;
        }

        if(fase_em_que_esta==1)
        {
        if(mudar)
        {
        VDP_drawImage(BG_B, &nuvem, 70, 10);
        VDP_drawImage(BG_B, &nuvem, 90, 2);
        VDP_drawImage(BG_B, &paleta_gelo, 0, 20);
        VDP_drawImage(BG_B, &paleta_gelo, 80, 20);
        VDP_drawImage(BG_B, &paleta_gelo, 300, 20);
        VDP_drawImage(BG_B, &agua, 40, 20);
        VDP_drawImage(BG_B, &agua, 20, 20);
        VDP_drawImage(BG_B, &lixo_retirada, 33, 12);
        PAL_setPalette(PAL0, player_sprite.palette->data, DMA);
        menos_lixo.Hitbox.largura = 32;
        menos_lixo.Hitbox.altura = 32;
        menos_lixo.Hitbox.x = 264;
        menos_lixo.Hitbox.y = 96;
        menos_lixos_bool = true;
        Lata.lixos = 0;
        Lata.Hitbox.x = 400;
        Lata.Hitbox.y = 150;
        Lata.Hitbox.largura = 16;
        Lata.Hitbox.altura = 16;
        lixo.Hitbox.x = 25;
        lixo.Hitbox.y = 150;
        lixo.Hitbox.largura = 16;
        lixo.Hitbox.altura = 16;
        lixo.x = 25;
        lixo.y = 150;

        Agua1.x = 240; 
        Agua1.y = 160;
        Agua1.largura = 32;
        Agua1.altura = 32;

        Agua2.x = 160; 
        Agua2.y = 160;
        Agua2.largura = 32;
        Agua2.altura = 32;

        player.x = 30;
        cam_x = 0;

        SPR_setFrame(Lata.sprite, LataDeLixo);
        SPR_setFrame(lixo.sprite, Lixo);
        SPR_setHFlip(Barata.sprite, TRUE);
        mudar = false;
        }
        Barata.vel = (random() % 2) + 3;
        mode2 = 0;
        lixos_necessarios = 5;
        }

if(fase_em_que_esta == 2){
    if(mudar)
    {
        Lata.lixos = 0;        
        lixos_necessarios = 30;
        cam_x = 0;
        mode2 = 1;
        mudar = false;

        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        VDP_setHorizontalScroll(BG_B, 0);
        SPR_reset();
        SPR_update();          
        SYS_doVBlankProcess(); 
        d = SPR_addSprite(&player2_sprite, 300, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        e = SPR_addSprite(&lata_sprite, 0, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        i = SPR_addSprite(&ball, 150, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        SPR_setFrame(i, WALK);
    }
}

            if(fase_em_que_esta==3)
        {
        if(mudar)
        {
       VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        VDP_setHorizontalScroll(BG_B, 0);
        SPR_reset();
        SPR_update();          
        player.sprite = SPR_addSprite(&player_sprite, player.x, player.y, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        Barata.sprite = SPR_addSprite(&barata_sprite, Barata.x, Barata.y, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        Lata.sprite = SPR_addSprite(&lata_sprite,  400, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        lixo.sprite = SPR_addSprite(&lixo_sprite, 25, 150, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
        VDP_drawImage(BG_B, &nuvem, 70, 10);
        VDP_drawImage(BG_B, &nuvem, 90, 2);
        VDP_drawImage(BG_B, &paleta_gelo, 0, 20);
        VDP_drawImage(BG_B, &paleta_gelo, 80, 20);
        VDP_drawImage(BG_B, &paleta_gelo, 300, 20);
        VDP_drawImage(BG_B, &agua, 40, 20);
        VDP_drawImage(BG_B, &agua, 20, 20);
        VDP_drawImage(BG_B, &lixo_retirada, 35, 12);
        PAL_setPalette(PAL0, player_sprite.palette->data, DMA);
        menos_lixo.Hitbox.largura = 32;
        menos_lixo.Hitbox.altura = 32;
        menos_lixo.Hitbox.x = 264;
        menos_lixo.Hitbox.y = 96;
        menos_lixos_bool = true;
        Lata.lixos = 0;
        Lata.Hitbox.x = 400;
        Lata.Hitbox.y = 150;
        Lata.Hitbox.largura = 16;
        Lata.Hitbox.altura = 16;
        lixo.Hitbox.x = 25;
        lixo.Hitbox.y = 150;
        lixo.Hitbox.largura = 16;
        lixo.Hitbox.altura = 16;
        lixo.x = 25;
        lixo.y = 150;

        Agua1.x = 240; 
        Agua1.y = 160;
        Agua1.largura = 32;
        Agua1.altura = 32;

        Agua2.x = 160; 
        Agua2.y = 160;
        Agua2.largura = 32;
        Agua2.altura = 32;

        player.x = 30;
        cam_x = 0;

        SPR_setFrame(Lata.sprite, LataDeLixo);
        SPR_setFrame(lixo.sprite, Lixo);
        SPR_setHFlip(Barata.sprite, TRUE);
        mudar = false;
        }
        Barata.vel = (random() % 2) + 4;
        mode2 = 0;
        lixos_necessarios = 20;
        } 

        if(mode2==0)
        {
        if(joy & BUTTON_RIGHT || joy & BUTTON_LEFT)
        {
            player.estado = 1;
            if(joy & BUTTON_RIGHT)
            {
                player.x += 3;
                SPR_setHFlip(player.sprite, FALSE);
            }
            if(joy & BUTTON_LEFT)
            {
                player.x -= 3;
                SPR_setHFlip(player.sprite, TRUE);
            }
            if(joy & BUTTON_A && player.pode_pular)
            {
                XGM_startPlayPCM(65, 15, SOUND_PCM_CH2);
                player.estado = 2;
                if(player.c_impulso_pulo < 16){
                    player.y -= 3;
                }
                player.c_impulso_pulo++;
            }
        }
        else if(joy & BUTTON_A && player.pode_pular)
        {
            XGM_startPlayPCM(65, 15, SOUND_PCM_CH2);
            player.estado = 2;
            if(player.c_impulso_pulo < 16){
                player.y -= 3;
            }
            player.c_impulso_pulo++;
        }
        else if(joy != BUTTON_RIGHT && joy != BUTTON_LEFT && joy != BUTTON_A && player.y == 150)
        {
            player.estado = 0;
        }

        if(player.estado == 0)
        {
            SPR_setFrame(player.sprite, PARADO);
        }
        if(player.estado == 1)
        {
            player.c_animacao += 8;
            if(player.c_animacao > 90)
            {
                if(!player.alternar_passo)
                {
                    SPR_setFrame(player.sprite, WALK);
                    player.alternar_passo = true;
                }
                else
                {
                    SPR_setFrame(player.sprite, WALK1);
                    player.alternar_passo = false;
                }
                player.c_animacao = 0;
            }
        }

        if(player.c_impulso_pulo < 16 && player.c_impulso_pulo > 0 && joy != BUTTON_A){
            player.c_impulso_pulo++;
            player.y -= 3;
        }
        if(player.c_impulso_pulo >= 16 && !player.fase_queda_rapida){
            player.c_topo_parabola++;
            player.c_impulso_pulo = 16;
            player.c_suave_subida++;
            if(player.c_suave_subida <= 9)
            {
                player.y--;
            }
        }
        if(player.c_topo_parabola >= 12 || player.esta_caindo_suave)
        {
            player.esta_caindo_suave = true;
            player.c_suave_queda++;
            if(player.c_suave_queda <= 9)
            {
                player.y++;
            }
            else{
                player.esta_caindo_suave = false;
                player.c_suave_queda = 0;
            }
            player.c_topo_parabola = 0;
            player.c_suave_subida = 0;
            player.fase_queda_rapida = true;
        }

        if(player.c_impulso_pulo >= 16 && player.fase_queda_rapida)
        {
            player.y += 3;
            if(player.y == 150)
            {
                player.c_suave_subida = 0;
                player.c_impulso_pulo = 0;
                player.c_trava_recarga = 0;
                player.c_topo_parabola = 0;
                player.c_suave_queda = 0;
                player.fase_queda_rapida = false;

                if(joy & BUTTON_A)
                {
                    player.pode_pular = false;
                }
            }
        }
        if(!player.pode_pular)
        {
            player.c_trava_recarga++;
        }
        if(player.c_trava_recarga > 20 && joy != BUTTON_A)
        {
            player.pode_pular = true;
            player.c_trava_recarga = 0;
        }
        if(player.estado == 2)
        {
            SPR_setFrame(player.sprite, JUMP);
        }

        if(checar_colisao(player.Hitbox, Agua1) || checar_colisao(player.Hitbox, Agua2))
        {
            player.y+=6;
        }
        else if(!checar_colisao(player.Hitbox, Agua1) && !checar_colisao(player.Hitbox, Agua2) && player.y > 150)
        {
            player.y=150;
        }

        if(Barata.x < 0)
        {
            SPR_setHFlip(Barata.sprite, FALSE);
            Barata.modo = true;
        }
        if(Barata.x > 300)
        {
            SPR_setHFlip(Barata.sprite, TRUE);
            Barata.modo = false;
        }
        if(player.x < 0)
        {
            player.x+=3;
        }
        if(player.x > limite)
        {
        player.x-=3;
        }
        if(Barata.modo == true)
        {
        Barata.x+=Barata.vel;
        }
        if(Barata.modo == false)
        {
        Barata.x-=Barata.vel;
        }
        Barata.c_animacao += 8;
        if(Barata.c_animacao > 90)
        {
        if(!Barata.alterar_passo)                
        {
        SPR_setFrame(Barata.sprite, barata);
        Barata.alterar_passo = true;
        }
        else
        {
        SPR_setFrame(Barata.sprite, barata2);
        Barata.alterar_passo = false;
        }
        Barata.c_animacao = 0;
        }

        player.Hitbox.x = player.x;
        player.Hitbox.y = player.y;
        
        Barata.Hitbox.x = Barata.x;
        Barata.Hitbox.y = Barata.y;

        contador++;

        if(contador>500)
        {
            if(modo==false)
            {
            modo=true;
            }
            else{
            modo = false;
            Barata.y=150;
            }
            contador=0;
        }

        if(modo)
        {
            if(!pular)
            {
            Barata.y-=Barata.vel;
            }
            else{
            Barata.y+=Barata.vel;
            }
        }
        if(Barata.y<0)
        {
            pular=true;
        }

        if(Barata.y>150)
        {
            pular=false;
        }

        if(checar_colisao(lixo.Hitbox, player.Hitbox))
        {
            segurando = true;
        }
        if(segurando)
        {
            lixo.Hitbox.x = player.x;
            lixo.Hitbox.y = player.y;
            lixo.y = player.y;
            lixo.x = player.x;
        }

        if(checar_colisao(lixo.Hitbox, Lata.Hitbox))
        {
            segurando = false;
            lixo.Hitbox.x = 20;
            lixo.Hitbox.y = 150;
            lixo.x = 25;
            lixo.y = 150;
            Lata.lixos++;
        }

        if(menos_lixos_bool)
        {
        if(menos_lixo.contador > 0)
        {
            menos_lixo.contador--;
        }
        if(checar_colisao(menos_lixo.Hitbox, player.Hitbox))
        {
            if(Lata.lixos > 0 && menos_lixo.contador==0)
            {
            Lata.lixos--;
            menos_lixo.contador=500;
            }
        }
        }


        Agua1.x = 480 - cam_x; 
        Agua1.y = 160;
        Agua1.largura = 52;
        Agua1.altura = 32;

        Agua2.x = 160 - cam_x; 
        Agua2.y = 160;
        Agua2.largura = 32;
        Agua2.altura = 32;

        cam_x = player.x - 160;

        if(checar_colisao(Barata.Hitbox, player.Hitbox) || checar_colisao(Agua1, player.Hitbox) && player.y>170 || checar_colisao(Agua2, player.Hitbox) && player.y>170)
        {
            mortes++;
            if(Lata.lixos >=1)
            {
            Lata.lixos--;
            }
            Barata.x=300;
            player.x=30;
            player.y = 150;
            Barata.y=150;
            player.c_impulso_pulo = 0;
            player.pode_pular=true;
        }
        if(Lata.lixos>=lixos_necessarios)
        {
            if(fase_em_que_esta==3)
            {
            drawTextCentered("FINAL DO JOGO", 13);
            }
            fase_em_que_esta++;
            mudar=true;
            SPR_clear();
            VDP_clearPlane(BG_A, TRUE);
        }
        if (cam_x < 0) cam_x = 0;
        if (cam_x > limite_cam) cam_x = limite_cam; 
        VDP_setHorizontalScroll(BG_B, -cam_x);
        VDP_setTextPalette(PAL1);
        sprintf(morte, "Mortes: %d", mortes);
        sprintf(Lixo1, "Lixos: %d/%d", Lata.lixos, lixos_necessarios);
        VDP_drawText(morte, x, y);
        VDP_drawText(Lixo1, x, y2);
        SPR_setPosition(player.sprite, player.x - cam_x, player.y);
        SPR_setPosition(Barata.sprite, Barata.x - cam_x, Barata.y);
        SPR_setPosition(Lata.sprite, Lata.Hitbox.x - cam_x, 150);
        SPR_setPosition(lixo.sprite, lixo.x - cam_x, lixo.y);
    }
    if(mode2 == 1)
        {
if (joy & BUTTON_UP || joy & BUTTON_DOWN) {
        if(joy & BUTTON_DOWN && joy & BUTTON_A)
        {
            if(d_y>=altura - 32)
            {
            goto pular;
            }
            d_y += 3; 
        }
        if(joy & BUTTON_UP && joy & BUTTON_A)
        {
            if(d_y<0)
            {
            goto pular;
            }
            d_y -= 3; 
        }
        if (joy & BUTTON_DOWN) {
            if(d_y>=altura - 32)
            {
            goto pular;
            }
            d_y += 2;
        } else {
        if(d_y<=0)
        {
        goto pular;
        }
            d_y -= 2;
        }
    }
        pular:
    if(mode  == false){
       i_x-=vel;
    }
    if(mode == true){
        i_x+=vel;
    }
    if(estado == 0){
        i_y++;
    }
    if(estado==1)
    {
        i_y--;
    }
    if(estado==2)
    {
        i_y-=2;
    }
    if(estado==3)
    {
        i_y+=2;
    }
    if (mode == true){
    if(checar_colisao(d_box, i_box)){
                if(estado < 3)
        {
            if(ea == 0){
                estado++;
            }
            if(ea == 1 && estado < 2){
                estado+=2;
            }
        }
        else{
            estado=0;
        }
        mode = false;
    if(estado == 0){
        i_y++;
    }
    if(estado==1)
    {
        i_y--;
    }
    if(estado==2)
    {
        i_y-=2;
    }
    if(estado==3)
    {
        i_y+=2;
    }
    }
    if(i_x > 320){
        Lata.lixos--;
        pontose++;
        i_x = 150;
        i_y = 150;
    }
    if(i_y <= 32){
        estado = 0;
    }
    if(i_y > altura - 32){
        estado = 1;
    }
}
    if (mode == false){
    if(checar_colisao(e_box, i_box)){
        if(estado < 3)
        {
            if(ea == 0){
                estado++;
            }
            if(ea == 1 && estado < 2){
                estado+=2;
            }
        }
        else{
            estado=0;
        }
                Lata.lixos++;
        mode = true;
    if(estado == 0){
        i_y++;
    }
    if(estado==1)
    {
        i_y--;
    }
    if(estado==2)
    {
        i_y-=2;
    }
    if(estado==3)
    {
        i_y+=2;
    }
    }
    if(i_x < 0){
        pontosd++;
        i_x = 150;
        i_y = 150;
    }
    if(i_y <= 32){
        estado = 0;
    }
    if(i_y > altura - 32){
        estado = 1;
    }
    if(i_y > e_y)
    {
        e_y+=2;
    }
    if(i_y < e_y)
    {
        e_y-=2;
    }
}
    d_box.y = d_y;
    e_box.y = e_y;
    i_box.y = i_y;
    i_box.x = i_x;
    u16 x = 20 - (strlen(texto) / 2);
    u16 y = 1;
    if(vel2==1000){
    vel+=0.02;
    vel2=0;
    }
    vel2++;
    if(ea<2)
    {
        ea++;
    }
    else{
        ea=0;
    }
    VDP_setTextPalette(PAL1);
    sprintf(texto, "Lixos: %d/%d", Lata.lixos, lixos_necessarios);
    VDP_drawText(texto, x, y);
    VDP_setTextPalette(PAL0);
    SPR_setPosition(d, 300, d_y);
    SPR_setPosition(e, 0, e_y);
    SPR_setPosition(i, i_x, i_y);
            VDP_setTextPalette(PAL1);

            SPR_setPosition(d, 300, d_y);
            SPR_setPosition(e, 0, e_y);
            SPR_setPosition(i, (s16)i_x, i_y);
                    if(Lata.lixos>=lixos_necessarios)
        {
            fase_em_que_esta++;
            mudar=true;
            SPR_clear();
            VDP_clearPlane(BG_A, TRUE);
        }
        }
        VDP_setTextPalette(PAL0);
        PAL_setColor(0, RGB24_TO_VDPCOLOR(0x0098E5));
        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}