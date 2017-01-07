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
#ifndef _MESSAGE_BOX_H_
#define _MESSAGE_BOX_H_

#include "gui/Gui.h"
	
class MessageBox : public GuiFrame, public sigslot::has_slots<>
{
public:	
	
	MessageBox(int typeButtons = BT_NOBUTTON, int typeIcons = IT_NOICON, bool progressbar = false);
	
	virtual ~MessageBox();
	
	void reload(std::string title, std::string message1, std::string message2, int typeButtons = BT_NOBUTTON, int typeIcons = IT_NOICON, bool progressBar = false, std::string pbInfo = " ");
	void setTitle(const std::string & title);
	void setMessage1(const std::string & message);
	void setMessage2(const std::string & message);
	void setProgress(f32 percent);
	void setProgressBarInfo(const std::string & info);
	
    sigslot::signal2<GuiElement *, int> messageCancelClicked;
	sigslot::signal2<GuiElement *, int> messageOkClicked;
	sigslot::signal2<GuiElement *, int> messageYesClicked;
	sigslot::signal2<GuiElement *, int> messageNoClicked;
	
	enum ButtonType
    {
		BT_NOBUTTON = -1,
		BT_OK,
		BT_CANCEL,
		BT_OKCANCEL,
		BT_YESNO,
		BT_DEST
    };
	
	enum IconType
    {
		IT_NOICON,
		IT_ICONTRUE,
		IT_ICONERROR,
		IT_ICONINFORMATION,
		IT_ICONQUESTION,
		IT_ICONEXCLAMATION,
		IT_ICONWARNING
    };
	
	enum MessageResult
	{
		MR_YES,
		MR_NO,
		MR_OK,
		MR_CANCEL
	};
	
private:
    void OnOkButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
    {
        messageOkClicked(this, MR_OK);
    }
	void OnCancelButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
    {
        messageCancelClicked(this, MR_CANCEL);
    }
	void OnYesButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
    {
        messageYesClicked(this, MR_YES);
    }
	void OnNoButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
    {
        messageNoClicked(this, MR_NO);
    }
	
	void setIcon(int typeIcons);
	void setButtons(int typeButtons);
	
	void OnReloadFadeOutFinished(GuiElement * element);
	void OnReloadFadeInFinished(GuiElement * element);
	
	void OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
	void UpdateButtons(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
	
	GuiImage bgBlur;
	
	GuiText titleText;
	GuiText messageText1;
	GuiText messageText2;
	GuiText infoText;
	
	GuiImageData *iconImageData;
    GuiImage * iconImage;
	
	GuiSound *buttonClickSound;
    GuiImageData *boxImageData;
    GuiImage boxImage;   
	
    GuiTrigger touchTrigger;
    GuiTrigger buttonATrigger;
    GuiTrigger buttonLeftTrigger;
    GuiTrigger buttonRightTrigger;
	
	GuiImageData *buttonImageData;
    GuiImageData *buttonHighlightedImageData;
	
	GuiImageData *bgImageData;
    GuiImage bgImage;
    GuiImage progressImageBlack;
    GuiImage progressImageColored;

    GuiButton DPADButtons;
	
	int selectedButtonDPAD;
	
	typedef struct
    {
        GuiImage *messageButtonImg;
        GuiImage *messageButtonHighlightedImg;
        GuiButton *messageButton;
        GuiText *messageButtonText;
    } MessageButton;

    std::vector<MessageButton> messageButtons;
	
    int buttonCount;
	bool progressBar;
	
	int newButtonsType;
	int newIconType;
	bool newProgressBar;
	std::string newTitle;
	std::string newMessage1;
	std::string newMessage2;
	std::string newInfo;
	
	GuiFrame progressFrame;
};

#endif //_MESSAGE_BOX_H_
