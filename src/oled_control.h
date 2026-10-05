void starting()
{
  oled.clearBuffer();
  oled.setFont(u8g2_font_roentgen_nbp_tr);

  oled.setCursor(10, 20);
  oled.printf("Starting...");
  oled.sendBuffer();
  delay(500);
}

// Para mostrar el texto asociado a una tecla
void drawInfoKey(byte number_key, const char *in_strKEY)
{
  switch (number_key)
  {
  case KEM_KEY1:
    oled.drawStr(5, 7, in_strKEY);
    break;
  case KEM_KEY2:
    oled.drawStr(5, 12, in_strKEY);
    break;
  case KEM_KEY3:
    oled.drawStr(55, 27, in_strKEY);
    break;
  case KEM_KEY4:
    oled.drawStr(100, 27, in_strKEY);
    break;
  case KEM_KEY5:
    oled.drawStr(55, 17, in_strKEY);
    break;
  case KEM_KEY6:
    oled.drawStr(100, 17, in_strKEY);
    break;
  case KEM_KEY7:
    oled.drawStr(55, 7, in_strKEY);
    break;
  case KEM_KEY8:
    oled.drawStr(100, 7, in_strKEY);
    break;

  default:
    break;
  }
}

void drawKEMKeys(byte x, byte y, byte size)
{
  oled.drawFrame(x, y, size, size);                                   // KEY1
  oled.drawFrame(x + (size + 2), y, size, size);                      // KEY2
  oled.drawFrame(x + (size * 2) + 4, y, size, size);                  // KEY3
  oled.drawFrame(x + (size * 3) + 6, y, size, size);                  // KEY4
  oled.drawFrame(x + (size * 2) + 4, y - (size + 2), size, size);     // KEY5
  oled.drawFrame(x + (size * 3) + 6, y - (size + 2), size, size);     // KEY6
  oled.drawFrame(x + (size * 2) + 4, y - (size * 2) - 4, size, size); // KEY7
  oled.drawFrame(x + (size * 3) + 6, y - (size * 2) - 4, size, size); // KEY8
}

void drawKEMKeys(byte x, byte y, byte size, byte key_press)
{
  drawKEMKeys(x, y, size);
  switch (key_press)
  {
  case KEM_KEY1:
    oled.drawBox(x, y, size, size); // KEY1
    break;
  case KEM_KEY2:
    oled.drawBox(x + (size + 2), y, size, size); // KEY2
    break;
  case KEM_KEY3:
    oled.drawBox(x + (size * 2) + 4, y, size, size); // KEY3
    break;
  case KEM_KEY4:
    oled.drawBox(x + (size * 3) + 6, y, size, size); // KEY4
    break;
  case KEM_KEY5:
    oled.drawBox(x + (size * 2) + 4, y - (size + 2), size, size); // KEY5
    break;
  case KEM_KEY6:
    oled.drawBox(x + (size * 3) + 6, y - (size + 2), size, size); // KEY6
    break;
  case KEM_KEY7:
    oled.drawBox(x + (size * 2) + 4, y - (size * 2) - 4, size, size); // KEY7
    break;
  case KEM_KEY8:
    oled.drawBox(x + (size * 3) + 6, y - (size * 2) - 4, size, size); // KEY8
    break;
  default:
    break;
  }
}

void drawLogo(byte speed, int pause)
{

  oled.clearBuffer();
  drawKEMKeys(105, 27, 4);

  oled.setFont(u8g2_font_calibration_gothic_nbp_t_all);
  oled.drawStr(5, 25, "K");
  oled.setFont(u8g2_font_mozart_nbp_t_all);
  oled.drawStr(65, 7, "Keyboard");
  oled.sendBuffer();
  delay(speed);

  oled.setFont(u8g2_font_calibration_gothic_nbp_t_all);
  oled.drawStr(18, 25, "E");
  oled.setFont(u8g2_font_mozart_nbp_t_all);
  oled.drawStr(65, 17, "Extender");
  oled.sendBuffer();
  delay(speed);

  oled.setFont(u8g2_font_calibration_gothic_nbp_t_all);
  oled.drawStr(31, 25, "- M");
  oled.setFont(u8g2_font_mozart_nbp_t_all);
  oled.drawStr(65, 27, "Maker");
  oled.sendBuffer();
  delay(speed);

  delay(pause);
}

void drawKEY(const char *in_strKEY)
{
  oled.clearBuffer();
  oled.setFont(u8g2_font_roentgen_nbp_tr);
  oled.drawStr(10, 10, in_strKEY);
  oled.sendBuffer();
}

void drawKEY(const char *in_strKEY, byte key_press)
{
  oled.clearBuffer();
  oled.setFont(u8g2_font_roentgen_nbp_tr);
  oled.drawStr(10, 10, in_strKEY);
  drawKEMKeys(98, 26, 6, key_press);
  oled.sendBuffer();
}

void drawVALUE(unsigned int in_VAL)
{
  oled.setFont(u8g2_font_chroma48medium8_8u);
  oled.setCursor(10, 25);
  oled.print(in_VAL);
  oled.sendBuffer();
}