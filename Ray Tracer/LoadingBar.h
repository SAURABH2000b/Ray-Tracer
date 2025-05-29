#pragma once
#include<string>
class LoadingBar
{
public:
	LoadingBar(char bgColor, char fgColor, int loadingBarWidth)
		:m_backgroundColor(bgColor),
		 m_foregroundColor(fgColor),
		 m_loadingBarWidth(loadingBarWidth){}

	void setBackgroundColor(char bgColor);
	void setForegroundColor(char fgColor);
	char getBackgroundColor();
	char getForegroundColor();
	int getLoadingBarWidth();

	void m_initialize(std::string title);
	void m_update();
	void m_finish();

private:
	char m_backgroundColor = '0';
	char m_foregroundColor = 'F';
	int m_loadingBarWidth = 25;
	char m_emptyBox = 177;
	char m_filledBox = 219;
	int m_loadStatus = 0;
};

