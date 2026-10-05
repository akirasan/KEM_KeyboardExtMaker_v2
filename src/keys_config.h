void setup_Keys()
{

/*

  KEM.setKeyFunc(KEM_KEY1, ADD_CUBE_BLENDER, "add cube");
  KEM.setKeyColorPress(KEM_KEY1, 255, 255, 255);

  KEM.setKey(KEM_KEY2, PIN_KEY2, LED_KEY2, MODE_HOLDED);
  KEM.setColorComand1(KEM_KEY2, 255, 0, 0);
  KEM.setColorComand2(KEM_KEY2, 0, 255, 0);
  KEM.setKeyFuncComand1(KEM_KEY2, Send_KEY2_func1, "func 1");
  KEM.setKeyFuncComand2(KEM_KEY2, Send_KEY2_func2, "func 2");

  KEM.setKeyFunc(KEM_KEY3, HelloWorld, "Hello World");
  KEM.setKeyColorPress(KEM_KEY3, 255, 255, 255);

  KEM.setKeyFunc(KEM_KEY4, ALT_TAB, "ALT + TAB");
  KEM.setKeyColorPress(KEM_KEY4, 255, 5, 5);

  KEM.setKeyColorPress(KEM_KEY5, 255, 255, 255);
  KEM.setKeyColorPress(KEM_KEY6, 255, 255, 255);
  KEM.setKeyColorPress(KEM_KEY7, 255, 255, 255);
  KEM.setKeyColorPress(KEM_KEY8, 255, 255, 255);

  KEM.setColorDefault(0, 0, 50);
  KEM.setColorBase(50, 0, 255);
  
  allColorBASE(50, 0, 150);
*/

KEM.setKey(KEM_KEY1, PIN_KEY1, LED_KEY1, MODE_PRESS, "Alt + TAB");
KEM.setKeyFunc(KEM_KEY1, ALT_TAB, "Alt + TAB");
KEM.setKeyColorPress(KEM_KEY1, 50,50,50);
KEM.setKeyColor(KEM_KEY1, 5,5,5);

KEM.setKeyName(KEM_KEY2, "Flameshot");
KEM.setKeyFunc(KEM_KEY2, CTR_WIN_S, "Flameshot");
KEM.setKeyColorPress(KEM_KEY2, 50,0,150);
KEM.setKeyColor(KEM_KEY2, 50,0,50);

KEM.setKeyName(KEM_KEY7, "ESC");
KEM.setKeyFunc(KEM_KEY7, ESC, "ESC");
KEM.setKeyColorPress(KEM_KEY7, 255, 0, 0);
KEM.setKeyColor(KEM_KEY7, 5,5,5);

KEM.setKeyFunc(KEM_KEY4, ALT_F4, "Alt + F4");
KEM.setKeyColor(KEM_KEY4, 55,0,0);
KEM.setKeyColorPress(KEM_KEY4, 55,55,0);

KEM.setKey(KEM_KEY8, PIN_KEY8, LED_KEY8, MODE_AFK,5,5,"AFK");
KEM.setColorComand1(KEM_KEY8, 0, 55, 0);
KEM.setColorComand2(KEM_KEY8, 55, 0, 0);
KEM.setKeyFuncComand1(KEM_KEY8, AFK_ON, "AFK ON");
KEM.setKeyFuncComand2(KEM_KEY8, AFK_ON, "AFK OFF");


allColorBASE(50, 0, 150);
}