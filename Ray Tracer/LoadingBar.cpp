#include "LoadingBar.h"
#include<iostream>
#include<string>
#include<windows.h>

void LoadingBar::setBackgroundColor(char bgColor)
{
    m_backgroundColor = bgColor;
}

void LoadingBar::setForegroundColor(char fgColor)
{
    m_foregroundColor = fgColor;
}

char LoadingBar::getBackgroundColor()
{
    return m_backgroundColor;
}

char LoadingBar::getForegroundColor()
{
    return m_foregroundColor;
}

int LoadingBar::getLoadingBarWidth()
{
    return m_loadingBarWidth;
}

void LoadingBar::m_initialize(std::string title)
{ 
    std::string s = "color ";
    s.push_back(m_backgroundColor);
    s.push_back(m_foregroundColor);
   
    system(s.c_str());
    std::clog << title << "\n";
    for (int i = 0; i < m_loadingBarWidth; i++) {
        std::clog << m_emptyBox;
        //printf("%c", m_emptyBox);
    }
    std::clog<<("\r");
}

void LoadingBar::m_update()
{
    if (m_loadStatus == m_loadingBarWidth)
        m_finish();
    m_loadStatus++;
    std::clog<<(m_filledBox);
}

void LoadingBar::m_finish()
{
    std::clog << "\n";
    m_loadStatus = 0;
    system("color 0F");
}
