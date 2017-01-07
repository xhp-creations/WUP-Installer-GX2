/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "BrowserWindow.h"

#define MAX_FOLDERS_PER_PAGE 3

BrowserWindow::BrowserWindow(int w, int h, CFolderList * list)
    : GuiFrame(w, h)
	, scrollbar(h - 150)
    , buttonClickSound(Resources::GetSound("button_click.mp3"))
    , buttonImageData(Resources::GetImageData("choiceUncheckedRectangle.png"))
    , buttonCheckedImageData(Resources::GetImageData("choiceCheckedRectangle.png"))
    , buttonHighlightedImageData(Resources::GetImageData("choiceSelectedRectangle.png"))
    , selectImageData(Resources::GetImageData("select_button.png"))
	, selectImg(selectImageData)
	, unselectImg(selectImageData)
	, installImg(selectImageData)
    , plusImageData(Resources::GetImageData("plus.png"))
    , minusImageData(Resources::GetImageData("minus.png"))
	, plusImg(plusImageData)
	, minusImg(minusImageData)
	, plusTxt("Select All", 42, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f))
	, minusTxt("Unselect All", 42, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f))
	, installTxt("Install", 42, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f))
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , buttonATrigger(GuiTrigger::CHANNEL_1, GuiTrigger::BUTTON_A, true)
    , buttonUpTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::BUTTON_UP | GuiTrigger::STICK_L_UP, true)
    , buttonDownTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::BUTTON_DOWN | GuiTrigger::STICK_L_DOWN, true)
    , plusTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::BUTTON_PLUS, true)
    , minusTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::BUTTON_MINUS, true)
    , DPADButtons(w,h)
    , AButton(w,h)
	, plusButton(selectImg.getWidth(), selectImg.getHeight())
	, minusButton(selectImg.getWidth(), selectImg.getHeight())
	, installButton(selectImg.getWidth(), selectImg.getHeight())
{
	folderList = list;
	currentYOffset = 0;
	
    buttonCount = folderList->GetCount();
	folderButtons.resize(buttonCount);
	
	for(int i = 0; i < buttonCount; i++)
	{      
		folderButtons[i].folderButtonImg = new GuiImage(buttonImageData);
		folderButtons[i].folderButtonCheckedImg = new GuiImage(buttonCheckedImageData);
		folderButtons[i].folderButtonHighlightedImg = new GuiImage(buttonHighlightedImageData);
		folderButtons[i].folderButton = new GuiButton(folderButtons[i].folderButtonImg->getWidth(), folderButtons[i].folderButtonImg->getHeight());
		folderButtons[i].folderButtonText = new GuiText(folderList->GetName(i).c_str(), 42, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
		
		folderButtons[i].folderButtonText->setMaxWidth(folderButtons[i].folderButtonImg->getWidth() - 70, GuiText::DOTTED);
		folderButtons[i].folderButtonText->setPosition(35, 0);
		
		folderButtons[i].folderButton->setImageSelectOver(folderButtons[i].folderButtonHighlightedImg);
		folderButtons[i].folderButton->setLabel(folderButtons[i].folderButtonText);
		folderButtons[i].folderButton->setSoundClick(buttonClickSound);
		folderButtons[i].folderButton->setImage(folderButtons[i].folderButtonImg);
		folderButtons[i].folderButton->setImageChecked(folderButtons[i].folderButtonCheckedImg);
		if(folderList->IsSelected(i))
			folderButtons[i].folderButton->check();
		
		folderButtons[i].folderButton->setPosition(0, 150 - (folderButtons[i].folderButtonImg->getHeight() + 30) * i);
		folderButtons[i].folderButton->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
        folderButtons[i].folderButton->setTrigger(&touchTrigger);
		folderButtons[i].folderButton->clicked.connect(this, &BrowserWindow::OnFolderButtonClick);
		
		this->append(folderButtons[i].folderButton);
	}
	
	if(buttonCount > MAX_FOLDERS_PER_PAGE)
    {
		scrollbar.SetPageSize((buttonImageData->getHeight() + 30) * (buttonCount - MAX_FOLDERS_PER_PAGE));
        scrollbar.SetEntrieCount((buttonImageData->getHeight() + 30) * (buttonCount - MAX_FOLDERS_PER_PAGE));
        scrollbar.setAlignment(ALIGN_RIGHT | ALIGN_MIDDLE);
        scrollbar.setPosition(0, -30);
        scrollbar.listChanged.connect(this, &BrowserWindow::OnScrollbarListChange);
        this->append(&scrollbar);
    }
	
	DPADButtons.setTrigger(&buttonUpTrigger);
    DPADButtons.setTrigger(&buttonDownTrigger);
    DPADButtons.clicked.connect(this, &BrowserWindow::OnDPADClick);
	this->append(&DPADButtons);
    
	AButton.setTrigger(&buttonATrigger);
    AButton.clicked.connect(this, &BrowserWindow::OnAButtonClick);
	this->append(&AButton);
	
	plusImg.setAlignment(ALIGN_BOTTOM | ALIGN_RIGHT);
	plusImg.setPosition(-10, 10);
    plusTxt.setMaxWidth(selectImg.getWidth()-5, GuiText::WRAP);
    plusButton.setLabel(&plusTxt);
    plusButton.setImage(&selectImg);
	plusButton.setIcon(&plusImg);
    plusButton.setAlignment(ALIGN_TOP | ALIGN_RIGHT);
    plusButton.setPosition(240, -135);
    plusButton.clicked.connect(this, &BrowserWindow::OnPlusButtonClick);
    plusButton.setTrigger(&plusTrigger);
    plusButton.setTrigger(&touchTrigger);
    plusButton.setSoundClick(buttonClickSound);
    plusButton.setEffectGrow();
    this->append(&plusButton);

    minusImg.setAlignment(ALIGN_BOTTOM | ALIGN_RIGHT);
	minusImg.setPosition(-10, 10);
	minusTxt.setMaxWidth(unselectImg.getWidth()-5, GuiText::WRAP);
    minusButton.setLabel(&minusTxt);
    minusButton.setImage(&unselectImg);
	minusButton.setIcon(&minusImg);
    minusButton.setAlignment(ALIGN_TOP | ALIGN_RIGHT);
    minusButton.setPosition(240, -(selectImg.getWidth()+20) - 135);
    minusButton.clicked.connect(this, &BrowserWindow::OnMinusButtonClick);
    minusButton.setTrigger(&minusTrigger);
    minusButton.setTrigger(&touchTrigger);
    minusButton.setSoundClick(buttonClickSound);
    minusButton.setEffectGrow();
    this->append(&minusButton);
	
	installTxt.setMaxWidth(unselectImg.getWidth()-5, GuiText::WRAP);
    installButton.setLabel(&installTxt);
    installButton.setImage(&installImg);
	installButton.setAlignment(ALIGN_TOP | ALIGN_RIGHT);
    installButton.setPosition(240, -(selectImg.getWidth()+20)*2 - 135);
    installButton.clicked.connect(this, &BrowserWindow::OnInstallButtonClick);
    installButton.setTrigger(&touchTrigger);
    installButton.setSoundClick(buttonClickSound);
    installButton.setEffectGrow();
    this->append(&installButton);
}

BrowserWindow::~BrowserWindow()
{
    for(u32 i = 0; i < folderButtons.size(); ++i)
    {
        delete folderButtons[i].folderButtonImg;
        delete folderButtons[i].folderButtonCheckedImg;
        delete folderButtons[i].folderButtonHighlightedImg;
        delete folderButtons[i].folderButton;
        delete folderButtons[i].folderButtonText;
    }
	
	folderButtons.clear();
   
    Resources::RemoveImageData(buttonImageData);
    Resources::RemoveImageData(buttonCheckedImageData);
    Resources::RemoveImageData(buttonHighlightedImageData);
    Resources::RemoveImageData(selectImageData);
    Resources::RemoveImageData(plusImageData);
    Resources::RemoveImageData(minusImageData);
    Resources::RemoveSound(buttonClickSound);
}

int BrowserWindow::SearchSelectedButton()
{
	int index = -1;
	for(int i = 0; i < buttonCount && index < 0; i++)
	{
		if(folderButtons[i].folderButton->getState() == STATE_SELECTED)
			index = i;
	}
	
	return index;
}

void BrowserWindow::OnFolderButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{	
	button->check();
	
	for(int i = 0; i < buttonCount; i++)
	{
		if(folderButtons[i].folderButton == button)
		{
			folderList->Click(i);
			folderButtons[i].folderButton->setState(STATE_SELECTED);
		}
		else
			folderButtons[i].folderButton->clearState(STATE_SELECTED);
	}
}

void BrowserWindow::OnAButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	int index = SearchSelectedButton();
	
	if(index < 0)
		return;
	
	folderList->Click(index);
	folderButtons[index].folderButton->check();
}

void BrowserWindow::OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	int index = SearchSelectedButton();
	
	if(index < 0 && trigger == &buttonUpTrigger)
		return;
	
	if(trigger == &buttonUpTrigger && index > 0)
	{
		folderButtons[index].folderButton->clearState(STATE_SELECTED);
		index--;
		folderButtons[index].folderButton->setState(STATE_SELECTED);
	}
	else if(trigger == &buttonDownTrigger && index < buttonCount-1)
	{
		if(index >= 0)
			folderButtons[index].folderButton->clearState(STATE_SELECTED);
		index++;
		folderButtons[index].folderButton->setState(STATE_SELECTED);
	}
	
	scrollbar.SetSelectedItem((buttonImageData->getHeight() + 30) * index);
}
	
void BrowserWindow::OnPlusButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	for(int i = 0; i < buttonCount; i++)
	{
		if(!folderList->IsSelected(i))
		{
			folderList->Select(i);
			folderButtons[i].folderButton->check();
		}
	}
}

void BrowserWindow::OnMinusButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	for(int i = 0; i < buttonCount; i++)
	{
		if(folderList->IsSelected(i))
		{
			folderList->UnSelect(i);
			folderButtons[i].folderButton->check();
		}
	}
}

void BrowserWindow::OnInstallButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
{
	installButtonClicked(this);
}

void BrowserWindow::OnScrollbarListChange(int selectItem, int pageIndex)
{
    currentYOffset = selectItem + pageIndex;

    for(int i = 0; i < buttonCount; i++)
    {
        folderButtons[i].folderButton->setPosition(0, 150 - (folderButtons[i].folderButtonImg->getHeight() + 30) * i + currentYOffset);
    }
}
