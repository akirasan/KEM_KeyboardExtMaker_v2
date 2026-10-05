// KEM Keyboard Extender Maker
// by akirasan
// septiembre 2021
// -------------------------------------------------

#include <Arduino.h>

#define MODE_PRESS 1  // Pulsación única, envía comando1
#define MODE_HOLDED 2 // Doble rol, comando1 / comando2
#define MODE_AFK 3    // Cómo HOLDED, pero envía cada cierto tiempo (segundos) el comando2

class KeyboardExtMaker
{

public:
    KeyboardExtMaker(class Adafruit_NeoPixel);
    void setKey(byte, byte, byte, byte, byte sensibilidad = 5, byte timer = 0, const char *texto = "KEY");
    void setKey(byte, byte, byte, byte, byte timer = 0, const char *texto = "KEY");
    void setKey(byte, byte, byte, byte, const char *texto = "KEY");
    void setKeyName(byte, const char *texto = "KEY");

    void setKeyFunc(byte, voidFuncPtr, const char *text_cmd1 = "KEY");
    void setKeyFuncComand1(byte, voidFuncPtr, const char *text_cmd1 = "KEY");
    void setKeyFuncComand2(byte, voidFuncPtr, const char *text_cmd2 = "KEY");

    void setColorDefault(byte, byte, byte);
    void setColorPress(byte, byte, byte, byte); // DEPRECATED
    void setColorComand1(byte, byte, byte, byte);
    void setColorComand2(byte, byte, byte, byte);
    void setColorBase(byte, byte, byte);
    void showColorKeys();
    void setKeyColor(byte, byte, byte, byte);
    void setKeyColorPress(byte, byte, byte, byte);

    boolean KeyPressed();

    void loop();

private:
    void drawInfoValor();

    typedef void (*voidFuncPtr)(void);
    typedef struct
    {
        byte PinKey;
        byte Led;
        byte Mode;
        byte Sensibilidad;
        byte Timer;             // Timer en segundos
        boolean Status = false; // Status marca en modo HOLDED / AFK el comando
        voidFuncPtr callfunc_cmd1;
        voidFuncPtr callfunc_cmd2;

        const char *text_cmd1;
        const char *text_cmd2;

        const char *name_key;

        byte R_st1;
        byte G_st1;
        byte B_st1;

        byte R_st2;
        byte G_st2;
        byte B_st2;

        boolean Keypressed;

        unsigned long timer1;
        unsigned long timer2;

        int16_t valor;

    } KEMkeys;

    typedef struct
    {
        byte R;
        byte G;
        byte B;
    } KEMbase;

    KEMkeys KeysKEM[8];
    KEMbase LedsBase[4];

    byte colorKeyDef_R;
    byte colorKeyDef_G;
    byte colorKeyDef_B;
};

//====================================================

KeyboardExtMaker::KeyboardExtMaker(class Adafruit_NeoPixel = led_key)
{

    // Definimos el básico de todo el teclado
    setKey(KEM_KEY1, PIN_KEY1, LED_KEY1, MODE_PRESS, 5, 0, "KEY 1");
    setKey(KEM_KEY2, PIN_KEY2, LED_KEY2, MODE_PRESS, 5, 0, "KEY 2");
    setKey(KEM_KEY3, PIN_KEY3, LED_KEY3, MODE_PRESS, 5, 0, "KEY 3");
    setKey(KEM_KEY4, PIN_KEY4, LED_KEY4, MODE_PRESS, 5, 0, "KEY 4");
    setKey(KEM_KEY5, PIN_KEY5, LED_KEY5, MODE_PRESS, 5, 0, "KEY 5");
    setKey(KEM_KEY6, PIN_KEY6, LED_KEY6, MODE_PRESS, 5, 0, "KEY 6");
    setKey(KEM_KEY7, PIN_KEY7, LED_KEY7, MODE_PRESS, 5, 0, "KEY 7");
    setKey(KEM_KEY8, PIN_KEY8, LED_KEY8, MODE_PRESS, 5, 0, "KEY 8");

    // Color base todo el teclado
    colorKeyDef_R = 0;
    colorKeyDef_G = 0;
    colorKeyDef_B = 0;
}

void KeyboardExtMaker::setColorDefault(byte red, byte green, byte blue)
{
    colorKeyDef_R = red;
    colorKeyDef_G = green;
    colorKeyDef_B = blue;
    allColorKEY(colorKeyDef_R, colorKeyDef_G, colorKeyDef_B);
}

void KeyboardExtMaker::showColorKeys()
{
    for (byte i = 0; i < 8; i++)
    {
        if (KeysKEM[i].Mode == MODE_HOLDED || KeysKEM[i].Mode == MODE_AFK)
        {
            if (KeysKEM[i].Status)
            {
                led_key.setPixelColor(KeysKEM[i].Led, KeysKEM[i].R_st1, KeysKEM[i].G_st1, KeysKEM[i].B_st1);
            }
            else
            {
                led_key.setPixelColor(KeysKEM[i].Led, KeysKEM[i].R_st2, KeysKEM[i].G_st2, KeysKEM[i].B_st2);
            }
        }
        else
        {
            if (KeysKEM[i].Mode == MODE_PRESS)
            {
                if (KeysKEM[i].R_st2 == 0 && KeysKEM[i].G_st2 == 0 && KeysKEM[i].B_st2 == 0)
                {
                    led_key.setPixelColor(KeysKEM[i].Led, colorKeyDef_R, colorKeyDef_G, colorKeyDef_B);
                }
                else
                {
                    led_key.setPixelColor(KeysKEM[i].Led, KeysKEM[i].R_st2, KeysKEM[i].G_st2, KeysKEM[i].B_st2);
                }
            }
        }
    }
    led_key.show();
}

// Interno para check y revisión
void KeyboardExtMaker::drawInfoValor()
{
    oled.clearBuffer();
    oled.setFont(u8g2_font_u8glib_4_tf);
    oled.setFont(u8g2_font_5x7_tf);

    oled.setCursor(10, 10);
    oled.print(KeysKEM[KEM_KEY7].valor);

    oled.setCursor(55, 10);
    oled.print(KeysKEM[KEM_KEY8].valor);

    oled.setCursor(10, 20);
    oled.print(KeysKEM[KEM_KEY5].valor);

    oled.sendBuffer();
}

void KeyboardExtMaker::loop()
{
    unsigned long tiempo1 = 0;
    unsigned long tiempo2 = 0;
    int16_t valor;
    bool showlogo = false;

    while (true)
    {
        tiempo2 = millis();

        for (byte i = 0; i < 8; i++)
        {
            KeysKEM[i].valor = valor = analogRead(KeysKEM[i].PinKey);

            if ((valor < 250) && (i < KEM_KEY8)) // KEY8 y KEY7 comparten PIN
            {
                KeysKEM[i].Keypressed = true;
                // delay(KeysKEM[i].Sensibilidad);
            }
            else
            {
                if ((valor > 250) && (valor < 1023) && (i == KEM_KEY8)) // KEY8 y KEY7 comparten PIN
                {
                    KeysKEM[i].Keypressed = true;
                    // delay(KeysKEM[i].Sensibilidad);
                }
            }
            delay(DELAY_PRESS);
            //drawInfoValor(); // para DEBUG / VERIFICACION
        }

        for (byte i = 0; i < 8; i++)
        {

            if ((KeysKEM[i].Keypressed) && (KeysKEM[i].Mode == MODE_PRESS))
            {
                KeysKEM[i].Keypressed = false;
                led_key.setPixelColor(KeysKEM[i].Led, KeysKEM[i].R_st1, KeysKEM[i].G_st1, KeysKEM[i].B_st1);
                led_key.show();
                drawKEY(KeysKEM[i].text_cmd1, i);
                if (KeysKEM[i].callfunc_cmd1 != NULL)
                {
                    KeysKEM[i].callfunc_cmd1();
                }

                delay(KeysKEM[i].Sensibilidad);
                tiempo1 = millis();
                showlogo = true;
            }
            else
            {
                if ((KeysKEM[i].Keypressed) && (KeysKEM[i].Mode == MODE_HOLDED))
                {
                    KeysKEM[i].Keypressed = false;
                    KeysKEM[i].Status = !KeysKEM[i].Status;
                    if (KeysKEM[i].Status)
                    {
                        drawKEY(KeysKEM[i].text_cmd1, i);
                        KeysKEM[i].callfunc_cmd1();
                    }
                    else
                    {
                        drawKEY(KeysKEM[i].text_cmd2, i);
                        KeysKEM[i].callfunc_cmd2();
                    }
                    delay(KeysKEM[i].Sensibilidad);
                    tiempo1 = millis();
                    showlogo = true;
                }
                else
                {
                    if ((KeysKEM[i].Keypressed) && (KeysKEM[i].Mode == MODE_AFK))
                    {
                        KeysKEM[i].Keypressed = false;
                        KeysKEM[i].Status = !KeysKEM[i].Status;
                        if (KeysKEM[i].Status) // Activamos el Timer de esta tecla
                        {
                            drawKEY(KeysKEM[i].text_cmd1, i);
                            KeysKEM[i].callfunc_cmd1();
                            KeysKEM[i].timer1 = millis();
                        }
                        else // Deactivamos el Timer de esta tecla
                        {
                            drawKEY(KeysKEM[i].text_cmd2, i);
                            KeysKEM[i].timer1 = 0;
                            KeysKEM[i].timer2 = 0;
                        }
                        showlogo = true;
                    }
                    else
                    {
                        if ((KeysKEM[i].Status) && (KeysKEM[i].Mode == MODE_AFK))
                        {
                            KeysKEM[i].timer2 = millis();
                            if ((KeysKEM[i].timer1 + (KeysKEM[i].Timer * 1000)) < KeysKEM[i].timer2)
                            {
                                drawKEY(KeysKEM[i].text_cmd1, i);
                                KeysKEM[i].callfunc_cmd1();
                                KeysKEM[i].timer1 = millis();
                                showlogo = true;
                            }
                        }
                    }
                }
            }
            showColorKeys();
        }

        if (((tiempo1 + 1000) < tiempo2) && showlogo)
        {
            drawLogo(0, 0);
            showlogo = false;
        }
    }
}

void KeyboardExtMaker::setKey(byte number_key, byte pin_key, byte led_position, byte mode, byte sensibilidad, byte timer, const char *texto)
{

    KeysKEM[number_key].Led = led_position;
    KeysKEM[number_key].PinKey = pin_key;

    KeysKEM[number_key].Status = 0;
    KeysKEM[number_key].Mode = mode;
    KeysKEM[number_key].Sensibilidad = sensibilidad;
    KeysKEM[number_key].Timer = timer;

    KeysKEM[number_key].text_cmd1 = texto;
    KeysKEM[number_key].text_cmd2 = texto;
    KeysKEM[number_key].name_key = texto;

    KeysKEM[number_key].callfunc_cmd1 = NULL;
    KeysKEM[number_key].callfunc_cmd2 = NULL;

    pinMode(KeysKEM[number_key].PinKey, INPUT_PULLUP);
}

void KeyboardExtMaker::setKey(byte number_key, byte pin_key, byte led_position, byte mode, byte timer, const char *texto)
{
    setKey(number_key, pin_key, led_position, mode, 5, timer, texto);
}

void KeyboardExtMaker::setKey(byte number_key, byte pin_key, byte led_position, byte mode, const char *texto)
{
    setKey(number_key, pin_key, led_position, mode, 5, 0, texto);
}

void KeyboardExtMaker::setKeyName(byte number_key, const char *texto)
{
    KeysKEM[number_key].name_key = texto;
}

// Define funcion al presionar MODO_PRESS
void KeyboardExtMaker::setKeyFunc(byte number_key, voidFuncPtr call_func, const char *texto)
{
    KeysKEM[number_key].callfunc_cmd1 = call_func;
    KeysKEM[number_key].text_cmd1 = texto;
}

// Define funcion al presionar estado 1 MODO_HOLDED
void KeyboardExtMaker::setKeyFuncComand1(byte number_key, voidFuncPtr call_func, const char *texto)
{
    KeysKEM[number_key].callfunc_cmd1 = call_func;
    KeysKEM[number_key].text_cmd1 = texto;
}

// Define funcion al presionar estado 1 MODO_HOLDED
void KeyboardExtMaker::setKeyFuncComand2(byte number_key, voidFuncPtr call_func, const char *texto)
{
    KeysKEM[number_key].callfunc_cmd2 = call_func;
    KeysKEM[number_key].text_cmd2 = texto;
}

boolean KeyboardExtMaker::KeyPressed()
{

    return false;
}

// Color al presionar MODO_PRESS
void KeyboardExtMaker::setKeyColorPress(byte number_key, byte red, byte green, byte blue)
{
    KeysKEM[number_key].R_st1 = red;
    KeysKEM[number_key].G_st1 = green;
    KeysKEM[number_key].B_st1 = blue;
}

void KeyboardExtMaker::setColorPress(byte number_key, byte red, byte green, byte blue) // DEPRECATED
{
    setKeyColorPress(number_key, red, green, blue);
}

// Color en estado 1 MODO_HOLDED
void KeyboardExtMaker::setColorComand1(byte number_key, byte red, byte green, byte blue)
{
    KeysKEM[number_key].R_st1 = red;
    KeysKEM[number_key].G_st1 = green;
    KeysKEM[number_key].B_st1 = blue;
}

// Color en estado 2 MODO_HOLDED
void KeyboardExtMaker::setColorComand2(byte number_key, byte red, byte green, byte blue)
{
    KeysKEM[number_key].R_st2 = red;
    KeysKEM[number_key].G_st2 = green;
    KeysKEM[number_key].B_st2 = blue;
}

// Color de toda la base
void KeyboardExtMaker::setColorBase(byte red, byte green, byte blue)
{
    for (byte i = 0; i < 4; i++)
    {
        LedsBase[i].R = red;
        LedsBase[i].G = green;
        LedsBase[i].B = blue;
    }
}

// Color por defecto de una tecla sin presionar para MODO_PRESS
void KeyboardExtMaker::setKeyColor(byte number_key, byte red, byte green, byte blue)
{
    // reutilizamos la definición del color del MODO_HOLDED / MODO_AFK
    setColorComand2(number_key, red, green, blue);
}