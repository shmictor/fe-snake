#pragma once

extern int windowIsOpen;

void openWindow(void);
char const* pollKeys(void);
double getDeltaTime(void);
void updateWindow(void);
void closeWindow(void);