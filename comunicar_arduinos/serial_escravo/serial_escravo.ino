/*
 * Protocolo
 * velocidade de comunicação 115200 
 * 
 * 0 => endereço de destino 0 a 255
 * 1 => Indica operacao 1=Entrega de endereço
 * 2 => Indica porta 
 * 3 => Indica valor 
 * Finaliza com newline '\n'
 * 
 * 0000\n Indica que existe um arduino acima
 * 
 * Todos os arduino ficam aguardando o endereço o primeiro arduino vai receber o endereco do master e então passa o endereço ao seguinte, que passa ao seguinte, ...
 * Entrega do primeiro endereço 0110\n
 * quando o master receber uma configuração de endereço,quer dizer que todos já estão conectados
 */

#define _MASTER_

#include <SoftwareSerial.h>

SoftwareSerial serial2( 3, 2 ); //RX, TX

byte cEndComunica = 0;
byte dadosRecebidos[10];
byte posiDado = 0;

void enviaEndereco() {
  byte endereco[4] = {0, 1, cEndComunica + 1, 0 };
  enviaCommando( endereco );
  delay(100);
  enviaCommando( endereco );
}

void enviaCommando( byte comm[] ) {
  for(byte i = 0; i < 4;i++){
    serial2.write(comm[i]); 
  }
  serial2.write('\n'); 
}

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Comunicacao com varios arduinos" );
  Serial.println( "ESCRAVO");
  
  serial2.begin( 115200 ); //Inicia porta comunicaçao serial entre arduinos  
}

void loop() {

  trataComandos();  
  
  //Restante das funcoes
  
}

void trataComandos() {
  if (serial2.available() > 0) {    
    byte charLido = serial2.read();
    Serial.println( "Recebido:" + String(charLido));
    if ( charLido == '\n' ) {      
      posiDado = 0;      
      if ( cEndComunica == 0 ) { //Recebendo endereço        
        if (( dadosRecebidos[0] == 0 )&&(dadosRecebidos[1] == 1)){
          cEndComunica = dadosRecebidos[2];
          Serial.println( "Recebido o endereo: " +String(cEndComunica));
          //Endereço recebido,passa o endereço para o seguinte
          enviaEndereco();
        }
      } else{
        //trata o comandos 
        Serial.println( "\ncomando" );
        if ( dadosRecebidos[0] == cEndComunica ) {
          pinMode( dadosRecebidos[2], OUTPUT );
          digitalWrite( dadosRecebidos[2], dadosRecebidos[3] );
        } else if ( dadosRecebidos[0] > 1 ) { //para o comando para o proximo arduino, tem que ser diferente de 1 que é o master
          enviaCommando( dadosRecebidos );
        }
      }
    } else{
      dadosRecebidos[posiDado++] = charLido;
    }
  }
}
