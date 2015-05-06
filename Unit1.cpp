//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#define cantx 28
#define canty 21
#include "Unit1.h"
#include "stdio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int mundo[cantx][canty], mundo2[cantx][canty]={0};
int PX1, PY1, PX2, PY2;
int MX, MY, MX2, MY2, NI=0, ND=0, toque=0, choque=0, subir, crear = 1;
int VID, PTO, CONT, NIV, POR, PTOANT = 0;
int CMOV = 10, CMOV2 = 10, poder = 0, Npoder, PXP, PYP, SUPERSONIC = 0;
struct fantasma
{  int PX, PY, TP, MX, MY, ES;
}f[6]={{19,11,0,-1,1,1},{3,9,1,1,-1,1},{1,17,3,0,-1,1},
       {23,8,0,-1,-1},{6,10,2,1,-1},{18,3,1,-1,-1}};
struct malonivel{
   int cantidad[4]; 
}Niveles[15]={ {{1,0,0,0}},
               {{1,0,0,1}},
               {{1,1,0,1}},
               {{1,1,0,2}},
               {{1,1,1,1}},   //Numero de fantasma para cada tipo de fantasma
               {{1,0,1,2}},
               {{2,1,0,2}},
               {{2,1,1,2}},
               {{1,2,0,2}},
               {{2,2,1,1}},
               {{1,1,2,2}},
               {{2,1,2,2}},
               {{2,2,2,2}},
               {{2,2,2,2}},
               {{2,2,3,2}},
               };
int NA = 3, NM=0, NI2=0, BC=0;
//--------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{  mundo_muevo();
   inicializar(1);                         //sonidos asignados
   Sonifondo->FileName = ".\\theme.mp3"; 
   Sonifondo->Open();

   Sonidopoder->FileName = ".\\poder.wav";
   Sonidopoder->Open();
   Sonidocomer->FileName = ".\\comer.wav";
   Sonidocomer->Open();
   Muerte->FileName = ".\\muerte.wav";
   Muerte->Open();
   final->FileName = ".\\final.wav";
   final->Open();
}
//---------------------------------------------------------------------------
void TForm1::Dibujarmundo() //pintado segun las filas y columnas, dimenciones 30x30
{  for (int j = 0; j < canty; j++)
        for (int i = 0; i < cantx; i++)
             bloque->Draw(PaintBox1->Canvas,i*30,j*30,mundo[i][j],1);
}
//---------------------------------------------------------------------------
void TForm1::mundo_muevo()
{  for (int i = 1; i < cantx-1; i++)
   {    mundo[i][1]=2;          mundo[i][canty-2]=2;
   }
   for (int j = 1; j < canty-1; j++)
   {    mundo[1][j]=2;          mundo[cantx-2][j]=2;
   }
   for (int j = 2; j < canty-2; j++)
        for (int i = 2; i < cantx-2; i++)
             mundo[i][j]=1;
   Dibujarmundo();
}
//---------------------------------------------------------------------------
void TForm1::inicializar(int todo)
{  PX1=30;  PY1=30;  PX2=30;   PY2=30;   MX2=0;   MY2=0;   NI=0;   ND=0;
   toque=0; choque=0;
   if (todo != 0){
      VID=2;  PTO=0; CONT=0; NIV=1, PTOANT = 0;
   }
   Label1->Caption = "Por = "+AnsiString(POR)+"/80";
   Label2->Caption = "x"+AnsiString(VID+1);
   Label3->Caption = "Puntaje = "+AnsiString(PTO);
   Label4->Caption = "NIVEL "+AnsiString(NIV);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1Paint(TObject *Sender)
{  Dibujarmundo();
   soni->Draw(PaintBox1->Canvas,PX1,PY1,NI+ND*3,1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{  MX=0;   MY=0;
   if (Key == 37)
       MX=-1;
   if (Key == 39)
       MX=1;
   if (Key == 38)
       MY=-1;
   if (Key == 40)
       MY=1;
}
//---------------------------------------------------------------------------
void TForm1::borrahorizontal(int x, int x2, int y)
{  if (x > x2)
       x+=20;
   x/=30;      x2/=30;     y/=30;
   bloque->Draw(PaintBox1->Canvas,x*30,y*30,mundo[x][y],1);
   bloque->Draw(PaintBox1->Canvas,x2*30,y*30,mundo[x2][y],1);
}
//---------------------------------------------------------------------------
void TForm1::borravertical(int y, int y2, int x)
{  if (y > y2)
       y+=20;
   y/=30;      y2/=30;     x/=30;
   bloque->Draw(PaintBox1->Canvas,x*30,y*30,mundo[x][y],1);
   bloque->Draw(PaintBox1->Canvas,x*30,y2*30,mundo[x][y2],1);
}
//---------------------------------------------------------------------------
void TForm1::borradiagonal(int x, int y, int x2, int y2)
{  if (x2 < x)
       x = x2;
   if (y2 < y)
       y = y2;
   bloque->Draw(PaintBox1->Canvas,x*30,y*30,mundo[x][y],1);
   bloque->Draw(PaintBox1->Canvas,(x+1)*30,y*30,mundo[(x+1)][y],1);
   bloque->Draw(PaintBox1->Canvas,x*30,(y+1)*30,mundo[x][(y+1)],1);
   bloque->Draw(PaintBox1->Canvas,(x+1)*30,(y+1)*30,mundo[(x+1)][(y+1)],1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{  NI2++;
   if (NI2 == 3)
       NI2 = 0;       //variaacion de sprites
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{  Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{  inicializar(1);
   Timer1->Enabled=true;
   Button1->Visible=false;
   Button2->Visible=false;
   Button3->Visible=false;
   Image1->Visible = false;
   PaintBox1->Visible = true;
   Timer1->Interval = 2000;
   Malos(0);
   Dibujarmundo();
   Sonifondo->Rewind();
   Sonifondo->Play();
}
//---------------------------------------------------------------------------
void TForm1::cambiabloque()
{  int mundo2[cantx][canty]={0}, bandera, contanterior=CONT;
   for (int j = 0; j < canty; j++)
       for (int i = 0; i < cantx; i++){
            if (mundo[i][j] >= 3){
                CONT++;
                mundo2[i][j]=2;
                mundo[i][j]=2;
                bloque->Draw(PaintBox1->Canvas,i*30,j*30,mundo[i][j],2);                                
            }else
                mundo2[i][j] = mundo[i][j];
       }
   for (int j = 0; j < NA; j++)
        if (f[j].TP == 0)
            mundo2[f[j].PX][f[j].PY] = 30;
   do{
      bandera = 0;
      for (int j = 0; j < canty; j++)
          for (int i = 0; i < cantx; i++)
              if (mundo2[i][j] == 30){
                  if (mundo2[i+1][j] == 1){
                      mundo2[i+1][j] = 30;
                      bandera = 1;
                  }
                  if (mundo2[i-1][j] == 1){
                      mundo2[i-1][j] = 30;
                      bandera = 1;
                  }
                  if (mundo2[i][j+1] == 1){
                      mundo2[i][j+1] = 30;
                      bandera = 1;
                  }
                  if (mundo2[i][j-1] == 1){
                      mundo2[i][j-1] = 30;
                      bandera = 1;
                  }
              }
   }while(bandera == 1);
   for (int j = 0; j < canty; j++)
        for (int i = 0; i < cantx; i++){
             if (mundo2[i][j] == 1){
                 mundo[i][j] = 2;
                 CONT++;
                 bloque->Draw(PaintBox1->Canvas,i*30,j*30,mundo[i][j],2);
             }
        }
   PTO += (CONT - contanterior)/6;
   this->Label3->Caption = "puntaje = " + AnsiString(PTO);
   //
   if (Timer4->Enabled == false && poder == 0 && random(1000)>800){
      Randomize();
      PXP = (random(cantx-2)+1)*30;
      PYP = (random(canty-2)+1)*30;      // ramdom de generar icono
      Npoder = random(4000)/1000;
      poder = 1;
//      Npoder = 2;
      Timer5->Enabled = true;
   }   
}
//---------------------------------------------------------------------------
int TForm1::choque_con_malos()
{  for (int i = 0; i < NA; i++)
       if (f[i].ES == 1 && f[i].PX == PX1/30 && f[i].PY == PY1/30){
           if (SUPERSONIC == 1){
               f[i].ES = 0;
               Timer6->Enabled = true;
               if (Efectos1->Checked == true)
                   Sonidocomer->Play();
               return 0;
           }
           return 1;
       }
   return 0;
}
//---------------------------------------------------------------------------
void TForm1::activar_toque(int x, int y)
{  if (mundo[x][y] == 3){
      mundo[x][y] = 4;
      toque = 1;
      bloque->Draw(PaintBox1->Canvas,x*30,y*30,mundo[x][y],4);
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{  int x = 0,y = 0, cambio, movio = 0;
   Timer1->Interval = 40;
   if ((MX != 0 || MY != 0) && PX1 == PX2 && PY1 == PY2){
        x=PX2/30 + MX;
        y=PY2/30 + MY;
        if (mundo[x][y] != 0)
            if (MX != -MX2 || MY != -MY2){
                PX2=x*30;         PY2=y*30;
                MX2=MX;           MY2=MY;
                MX=0;             MY=0;
                if (MX2 != 0)
                    if (MX2 > 0)
                        ND=0;
                    else
                        ND=1;
                else
                    if (MY2 > 0)
                        ND=2;
                    else
                        ND=3;
            }else{
                PX2=PX2 + MX2*30;
                PY2=PY2 + MY2*30;
            }
   }else{
        if (PX1 != PX2){
            borrahorizontal(PX1, PX2, PY1);
            x=(PX2-PX1)/abs(PX2-PX1);
            PX1+=CMOV*x;
            if (PX1 == PX2){
                Revisar_poder();
                NI=0;
                if (mundo[PX2/30][PY2/30] == 1){
                    mundo[PX2/30][PY2/30] = 3;
                    bloque->Draw(PaintBox1->Canvas,PX2,PY2,3,1);
                    if (MX == 0 && MY == 0)
                      PX2+=30*x;
                }else
                    if (revizar(mundo[PX2/30][PY2/30], mundo[PX2/30-x][PY2/30-y]))
                        return;
                movio=0;
            }else
                movio++;
            if (movio > 2)
                movio = 0;
            if (SUPERSONIC == 1)
               SONIC_SUPER->Draw(PaintBox1->Canvas,PX1,PY1,movio+ND*3,1);
            else
                soni->Draw(PaintBox1->Canvas,PX1,PY1,movio+ND*3,1);

            NI++;
        }
        if (PY1 != PY2){
            borravertical(PY1, PY2, PX1);
            y=(PY2-PY1)/abs(PY2-PY1);
            PY1+=CMOV*y;
            if (PY1 == PY2){
                Revisar_poder();
                NI=0;
                if (mundo[PX2/30][PY2/30] == 1){
                    mundo[PX2/30][PY2/30] = 3;
                    bloque->Draw(PaintBox1->Canvas,PX2,PY2,3,1);
                    if (MX == 0 && MY == 0)
                        PY2+=30*y;
                }else
                    if (revizar(mundo[PX2/30][PY2/30], mundo[PX2/30-x][PY2/30-y]))
                        return;
                movio=0;
            }else
                movio++;
            if (movio > 2)
                movio = 0;
            if (SUPERSONIC == 1)
               SONIC_SUPER->Draw(PaintBox1->Canvas,PX1,PY1,movio+ND*3,1);
            else
                soni->Draw(PaintBox1->Canvas,PX1,PY1,movio+ND*3,1);

            NI++;
        }
   }
   /////////////////////////////////////////////////////////////////////////////
   for (int i = 0; i < NA; i++){
      if (f[i].ES == 1){
        if (f[i].TP == 0 || f[i].TP == 1)
            bloque->Draw(PaintBox1->Canvas,f[i].PX*30+NM*f[i].MX,f[i].PY*30+NM*f[i].MY,1,1);
        if (f[i].TP == 2)
            borradiagonal(f[i].PX, f[i].PY, f[i].PX+f[i].MX, f[i].PY+f[i].MY);
        if (f[i].TP == 3){
            if (f[i].MX != 0)
                borrahorizontal(f[i].PX*30,(f[i].PX+f[i].MX)*30,f[i].PY*30);
            else
                borravertical(f[i].PY*30,(f[i].PY+f[i].MY)*30,f[i].PX*30);
        }
      }  
   }
// 0 vacio
// 1 vacio comer
// 2 lleno
// 3 bordeador
   revisar_toque();
   if (NM < 30){
       NM+=CMOV2;
   }else{
       NM = 0;
       if (POR >= 20 && crear > 0){
           crear = 0;

       }
       for (int j = 0; j < canty; j++)
          for (int i = 0; i < cantx; i++)
               mundo2[i][j] = 0;
       for (int i = 0; i < NA; i++){
          if(f[i].ES == 1){
             f[i].PX+=f[i].MX;
             f[i].PY+=f[i].MY;
             mundo2[f[i].PX][f[i].PY]=1;
             mundo2[(f[i].PX+f[i].MX)][(f[i].PY+f[i].MY)]++;
          }
       }
       for (int i = 0; i < NA; i++)
          if(f[i].ES == 1){
             if (f[i].TP == 0 || f[i].TP == 1)
                 rebotar(i,1);
             if (f[i].TP == 2)
                 rebotar(i,2);
             if (f[i].TP == 3)
                 bordear(i);
          }
   }
   for (int i = 0; i < NA; i++)
       if(f[i].ES == 1)
          malo->Draw(PaintBox1->Canvas,f[i].PX*30+NM*f[i].MX,f[i].PY*30+NM*f[i].MY,NI2+3*f[i].TP,1);

      if (SUPERSONIC == 1)
         SONIC_SUPER->Draw(PaintBox1->Canvas,PX1,PY1,movio+ND*3,1);
      else
         soni->Draw(PaintBox1->Canvas,PX1,PY1,movio+ND*3,1);
//   movio++;
   if (poder == 1){
      Poder->Draw(PaintBox1->Canvas,PXP,PYP,Npoder,1);
   }   
   revisar_a_Sonic();
   if (Sonifondo->Length == Sonifondo->Position){
      Sonifondo->Rewind();
      Sonifondo->Play();
   }

}           
//---------------------------------------------------------------------------
int TForm1::revizar(int actual, int anterior)
{
   if (actual == 2 && anterior >= 3){
       toque = 0;
       cambiabloque();
       POR = CONT*100/((cantx-4)*(canty-4));
       Label1->Caption = "por = "+AnsiString(POR)+"/80";
       if (POR > 70){
           POR = 0;
           CONT = 0;
           if (Efectos1->Checked == true)
              final->Play();
           if (NIV < 15){
               NIV++;
               PTOANT = PTO;
               ShowMessage("En Buena Hora Superaste El Nivel");
               inicializar(0);
               mundo_muevo();
               Malos(NIV-1);
           }else{
               ShowMessage("Has Finalizado El Juego");
               Timer1->Enabled = false;
               Button1->Visible = true;
               Button2->Visible = true;
               Button3->Visible = true;
               Image1->Visible = true;
               PaintBox1->Visible = false;
               inicializar(0);
               mundo_muevo();
               return 1;
           }
       }    
   }
   if (actual >= 3)
       choque = 1;
   MX2 = 0;    MY2 = 0;
   return 0;
}
//---------------------------------------------------------------------------
void TForm1::rebotar(int N, int O)//REBOTADORES
{  int cambio, x, y, Ncambios = 0;
   do{
      x = f[N].PX + f[N].MX;
      y = f[N].PY + f[N].MY;
      cambio = 0;
      if (mundo[x][f[N].PY] != O || mundo2[x][f[N].PY] > 0){
          activar_toque(x, f[N].PY);
          if (f[N].TP == 1 && (x > 2 && x < cantx-2) && mundo[x][f[N].PY] == 2){
              mundo[x][f[N].PY] = 1;
              bloque->Draw(PaintBox1->Canvas,x*30,f[N].PY*30,1,1);
          }
          f[N].MX*=(-1);
          cambio = 1;
      }
      if (mundo[f[N].PX][y] != O || mundo2[f[N].PX][y] > 0){
          activar_toque(f[N].PX, y);
          if (f[N].TP == 1 &&  (f[N].PY > 2 && f[N].PY < canty-3) && mundo[f[N].PX][y] == 2){
              mundo[f[N].PX][y] = 1;
              bloque->Draw(PaintBox1->Canvas,f[N].PX*30,y*30,1,1);
          }
          f[N].MY*=(-1);
          cambio = 1;
      }
      if (cambio == 0 && (mundo[x][y] != O || mundo2[x][y] > 1)){
          activar_toque(x, y);
          if (f[N].TP == 1 && (x > 2 && x < cantx-2 && y > 2 && y < canty-2) && mundo[x][y] == 2){
              mundo[x][y] = 1;
              bloque->Draw(PaintBox1->Canvas,x*30,y*30,1,1);
          }
          f[N].MX*=(-1);
          f[N].MY*=(-1);
          cambio=1;
      }
      Ncambios++;
   }while (cambio == 1 && Ncambios < 5);
}
//---------------------------------------------------------------------------
// x -1, 0, 1, 0
// y  0,-1, 0, 1
void TForm1::bordear(int N)
{  int x, y, t, cont = 4;
   t = f[N].MX;
   f[N].MX = -f[N].MY;
   f[N].MY = t;
   do{
      x = f[N].PX + f[N].MX;
      y = f[N].PY + f[N].MY;
      if (mundo[x][y] != 2){
         t = f[N].MY;
         f[N].MY = -f[N].MX;
         f[N].MX = t;
         cont--;
         activar_toque(x, y);
      }else
         return;
   }while(cont > 0);
}
//---------------------------------------------------------------------------
void TForm1::revisar_toque()
{  if (toque == 1){
       for (int j = 1; j < canty-1; j++)
          for (int i = 1; i < cantx-1; i++)
               if (mundo[i][j] == 4)
                   mundo[i][j] = 5;
       for (int j = 1; j < canty-1; j++)
          for (int i = 1; i < cantx-1; i++)
               if (mundo[i][j] == 5){
                   activar_toque(i+1,j);
                   activar_toque(i-1,j);
                   activar_toque(i,j+1);
                   activar_toque(i,j-1);
               }
   }
}
//---------------------------------------------------------------------------
void TForm1::revisar_a_Sonic()
{  int a = PX1/30, b = PY1/30, c;
   if (mundo[a][b] > 3 || choque == 1 || choque_con_malos()){
       if (SUPERSONIC == 0){
          Timer1->Enabled = false;
          Timer3->Enabled = true;
          if (Efectos1->Checked == true)
             Muerte->Play();
          subir = 6;
       }
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer3Timer(TObject *Sender)
{  int a, b;
   if (subir > 0){
       subir--;
       PY1-=30;
   }else{
       PY1+=30;
   }
   a = PX1/30;          b = PY1/30;
   for (int j = b-1; j < b+3; j++)
        for (int i = a; i < a+2; i++)
             bloque->Draw(PaintBox1->Canvas,i*30,j*30,mundo[i][j],1);
   for (int i = 0; i < NA; i++)
       if(f[i].ES == 1)
           malo->Draw(PaintBox1->Canvas,f[i].PX*30+NM*f[i].MX,f[i].PY*30+NM*f[i].MY,NI2+3*f[i].TP,1);
   soni3->Draw(PaintBox1->Canvas,PX1,PY1,0,1);
   if (PY1 > 1000){
      Timer3->Enabled = false;
      if (VID > 0){
          VID--;
          inicializar(0);
          for (int j = 0; j < canty; j++)
              for (int i = 0; i < cantx; i++)
                  if (mundo[i][j] > 2){
                     mundo[i][j] = 1;
                     bloque->Draw(PaintBox1->Canvas,i*30,j*30,1,1);
                  }
          Timer1->Enabled = true;
          Revisar_esquina();
      }else{
          ShowMessage("Has Perdido INtentalo de Nuevo");
          Timer1->Enabled = false;
          Button1->Visible = true;
          Button2->Visible = true;
          Button3->Visible = true;
          Image1->Visible = true;
          PaintBox1->Visible = false;
          Sonifondo->Stop();
          mundo_muevo();
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer4Timer(TObject *Sender)
{  Timer4->Enabled = 0;
   Quitar_poder();    //quitar efecto de poder
}
//---------------------------------------------------------------------------
void TForm1::Quitar_poder()
{  SUPERSONIC = 0;
   CMOV  = 10;
   CMOV2 = 10;
   poder = 0;
}
//---------------------------------------------------------------------------
void TForm1::Revisar_poder()
{  if (poder == 1 && PXP == PX1 && PYP == PY1){
       if (Efectos1->Checked == true)
          Sonidopoder->Play();
       if (Npoder == 0){
           SUPERSONIC = 1;
       }
       if (Npoder == 1)
           CMOV = 15;
       if (Npoder == 2)           //ejcutar poder
                                   // mov= movimiento de sonix
           CMOV2 = 2;               // mov2= movimientio de los fantasmas
       if (Npoder == 3)
           CMOV2 = 0;
       poder = 0;
       Timer4->Enabled = 1;
   }
}

void __fastcall TForm1::Timer5Timer(TObject *Sender)
{  Timer5->Enabled = false;              //pqitar poder el icono del poder la imagen
   poder = 0;
   bloque->Draw(PaintBox1->Canvas,PXP,PYP,mundo[PXP/30][PYP/30],2);
}    //borrar icono de poder
//---------------------------------------------------------------------------
void TForm1::Revisar_esquina()
{  for (int i = 0; i < NA; i++)
          if(f[i].ES == 1){
              if (f[i].TP == 2)
                  f[i].ES = 0;
              if (f[i].TP == 3){
                  if (random(1) == 1){
                     f[i].PX = random(cantx - 9) + 7;
                     f[i].MY = 0;
                     if (random(1) == 1){
                         f[i].MX = 1;
                         f[i].PY = 1;
                     }else{
                        f[i].MX = -1;
                        f[i].PY = canty - 2;
                     }
                  }else{
                     f[i].PY = random(canty - 9) + 7;
                     f[i].MX = 0;
                     if (random(1) == 1){
                         f[i].MY = -1;
                         f[i].PX = 1;
                     }else{
                         f[i].MY = 1;
                         f[i].PX = cantx - 2;
                     }
                  }
              }
          }
   Dibujarmundo();          
}
//---------------------------------------------------------------------------
void TForm1::Malos(int Nivel)
{ int cont = 0, N;
  int mundo3[cantx][canty]={0};
  Randomize();
  for (int i = 0; i < 3; i++)
  {   if (Niveles[Nivel].cantidad[i] > 0){
         for (int j = 0; j < Niveles[Nivel].cantidad[i]; j++){
              f[cont].TP = i;
              if (i == 2){
                 f[cont].ES = 0;
                 f[cont].PX = 100;   //para el fantasma q´rebota en lo lleno
                 f[cont].PY = 100;
              }else{
                 f[cont].ES = 1;           //posiciones fantasmas
                 do{
                    f[cont].PX = (random(6) + 1)*4;
                    f[cont].PY = (random(6) + 1)*3;
                 }while(mundo3[f[cont].PX][f[cont].PY] == 1);
                 if (random(1) == 1)
                     f[cont].MX = 1;
                 else
                     f[cont].MX = -1;        //direccione del fantasma
                 if (random(1) == 1)
                     f[cont].MY = 1;
                 else
                     f[cont].MY = -1;
                 mundo3[f[cont].PX][f[cont].PY] = 1;
              }
              cont++;
         }
      }
  }
  if (Niveles[Nivel].cantidad[3] > 0)
      for (int j = 0; j < Niveles[Nivel].cantidad[3]; j++){
          f[cont].TP = 3;
          f[cont].ES = 1;           //posiciones fantasmas
          do{
              if (random(1) == 1){
                  f[cont].PX = random(cantx - 9) + 7;
                  f[cont].MY = 0;
                  if (random(1) == 1){
                      f[cont].MX = 1;
                      f[cont].PY = 1;
                  }else{
                      f[cont].MX = -1;
                      f[cont].PY = canty - 2;
                  }
              }else{
                  f[cont].PY = random(canty - 9) + 7;
                  f[cont].MX = 0;
                  if (random(1) == 1){
                      f[cont].MY = -1;
                      f[cont].PX = 1;
                  }else{
                      f[cont].MY = 1;
                      f[cont].PX = cantx - 2;
                  }
              }
          }while(mundo3[f[cont].PX][f[cont].PY] == 1);
          mundo3[f[cont].PX][f[cont].PY] = 1;
          cont++;
      }
   NA = cont;
}

void __fastcall TForm1::Timer6Timer(TObject *Sender)  //para revivir los fantasmas
{ Timer6->Enabled = false;
  int N;
  int mundo3[cantx][canty]={0};
  Randomize();
  for (int i = 0; i < NA; i++)
  {   if (f[i].ES == 1)
          mundo3[f[i].PX][f[i].PY] == 1;
      if (f[i].ES == 0 && f[i].TP != 2){
          f[i].ES = 1;           //posiciones fantasmas
          if (f[i].TP < 2){
             do{
                f[i].PX = (random(6) + 1)*4;
                f[i].PY = (random(6) + 1)*3;
             }while(mundo3[f[i].PX][f[i].PY] == 1 || mundo[f[i].PX][f[i].PY] == 1);
             if (random(1) == 1)
                 f[i].MX = 1;
             else
                 f[i].MX = -1;        //direccione del fantasma
             if (random(1) == 1)
                 f[i].MY = 1;
             else
                 f[i].MY = -1;
             mundo3[f[i].PX][f[i].PY] = 1;
          }else{
             do{
               if (random(1) == 1){
                  f[i].PX = random(cantx - 9) + 7;
                  f[i].MY = 0;
                  if (random(1) == 1){
                      f[i].MX = 1;
                      f[i].PY = 1;
                  }else{
                      f[i].MX = -1;
                      f[i].PY = canty - 2;
                  }
               }else{
                  f[i].PY = random(canty - 9) + 7;
                  f[i].MX = 0;
                  if (random(1) == 1){
                      f[i].MY = -1;
                      f[i].PX = 1;
                  }else{
                      f[i].MY = 1;
                      f[i].PX = cantx - 2;
                  }
               }
             }while(mundo3[f[i].PX][f[i].PY] == 1);
             mundo3[f[i].PX][f[i].PY] = 1;
          }
      }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer7Timer(TObject *Sender)
{  int mundo3[cantx][canty]={0}, cont = 0;
 if (Timer1->Enabled == true && POR > 15)
   for (int i = 0; i < NA; i++)
   {   if (f[i].ES == 1)
          mundo3[f[i].PX][f[i].PY] == 1;
       if (f[i].ES == 0 && f[i].TP == 2){
          f[i].ES = 1;           //posiciones fantasmas
          do{
              f[i].PX = (random(6) + 1)*4;
              f[i].PY = (random(6) + 1)*3;
              cont++;
              if (cont >= 30){
                  for (int j = 5; j < 25; j++)
                      for (int k = 5; k < 15; k++){
                          if (mundo3[k][j] == 0 && mundo[f[i].PX][f[i].PY]== 2){
                              f[i].PX = k;
                              f[i].PY = j;
                              j = 30;
                              k = 30;
                          }
                      }
                  break;
              }
          }while(mundo3[f[i].PX][f[i].PY] == 1 || mundo[f[i].PX][f[i].PY] != 2);
          if (random(1) == 1)
              f[i].MX = 1;
          else
              f[i].MX = -1;        //direccione del fantasma
          if (random(1) == 1)
              f[i].MY = 1;
          else
              f[i].MY = -1;
           mundo3[f[i].PX][f[i].PY] = 1;
       }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Fondo1Click(TObject *Sender)
{  if (Fondo1->Checked == true && Timer1->Enabled == true)
       Sonifondo->Play();
   if (Fondo1->Checked == false)
       Sonifondo->Stop();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Salir1Click(TObject *Sender)
{  Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::guardar1Click(TObject *Sender)
{  FILE *archivo;
   SaveDialog1->InitialDir = ".\\";
   if (Timer1->Enabled == true && SaveDialog1->Execute()){
       archivo = fopen(SaveDialog1->FileName.c_str(),"w+");
       fprintf(archivo, "%d %d %d",NIV,VID, PTOANT);
       fclose(archivo);
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::cargar1Click(TObject *Sender)
{  FILE *archivo;
   OpenDialog1->InitialDir = ".\\";
   if (OpenDialog1->Execute()){
       archivo = fopen(OpenDialog1->FileName.c_str(),"r");
       fscanf(archivo, "%d%d%d",&NIV,&VID,&PTO);
       fclose(archivo);
       inicializar(0);
       Timer1->Enabled=true;
       Button1->Visible=false;
       Button2->Visible=false;
       Button3->Visible=false;
       Image1->Visible = false;             //click juego y sub clips

       PaintBox1->Visible = true;
       Timer1->Interval = 2000;
       Malos(NIV-1);
       Dibujarmundo();
       Sonifondo->Rewind();
       Sonifondo->Play();
   }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button3Click(TObject *Sender)
{
FILE *archivo;
   OpenDialog1->InitialDir = ".\\";
   if (OpenDialog1->Execute()){
       archivo = fopen(OpenDialog1->FileName.c_str(),"r");
       fscanf(archivo, "%d%d%d",&NIV,&VID,&PTO);
       fclose(archivo);
       inicializar(0);
       Timer1->Enabled=true;
       Button1->Visible=false;
       Button2->Visible=false;
       Button3->Visible=false;
       Image1->Visible = false;             //click juego y sub clips

       PaintBox1->Visible = true;
       Timer1->Interval = 2000;
       Malos(NIV-1);
       Dibujarmundo();
       Sonifondo->Rewind();
       Sonifondo->Play();
   }
}
//---------------------------------------------------------------------------



