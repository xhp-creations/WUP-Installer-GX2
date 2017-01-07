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
#include "MainWindow.h"
#include "Application.h"
#include "utils/StringTools.h"
#include "common/retain_vars.h"
#include "common/common.h"
#include "common/svnrev.h"
#include "gui/MessageBox.h"

MainWindow::MainWindow(int w, int h)
	: width(w)
	, height(h)
	, bgParticleImg(w, h, 500)
	, splashImgData(Resources::GetImageData("splash.png"))
	, splashImg(splashImgData)
	, titleImgData(Resources::GetImageData("titleHeader.png"))
	, titleImg(titleImgData)
	, titleText("WUP Installer GX2")
	, versionText(WUP_GX2_VERSION)
{
	folderList = NULL;
	installWindow = NULL;
	
	for(int i = 0; i < 4; i++)
	{
		std::string filename = strfmt("player%i_point.png", i+1);
		pointerImgData[i] = Resources::GetImageData(filename.c_str());
		pointerImg[i] = new GuiImage(pointerImgData[i]);
		pointerImg[i]->setScale(1.5f);
		pointerValid[i] = false;
	}
	
	SetupMainView();
}

MainWindow::~MainWindow()
{
	remove(&bgParticleImg);
	Resources::RemoveImageData(splashImgData);
	Resources::RemoveImageData(titleImgData);
	
	while(!tvElements.empty())
	{
		delete tvElements[0];
		remove(tvElements[0]);
	}
	while(!drcElements.empty())
	{
		delete drcElements[0];
		remove(drcElements[0]);
	}
	for(int i = 0; i < 4; i++)
	{
		delete pointerImg[i];
		Resources::RemoveImageData(pointerImgData[i]);
	}
	
	if(folderList != NULL)
		delete folderList;
}

void MainWindow::updateEffects()
{
	//! dont read behind the initial elements in case one was added
	u32 tvSize = tvElements.size();
	u32 drcSize = drcElements.size();
	
	for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
	{
		drcElements[i]->updateEffects();
	}
	
	//! only update TV elements that are not updated yet because they are on DRC
	for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
	{
		u32 n;
		for(n = 0; (n < drcSize) && (n < drcElements.size()); n++)
		{
			if(tvElements[i] == drcElements[n])
				break;
		}
		if(n == drcElements.size())
		{
			tvElements[i]->updateEffects();
		}
	}
}

void MainWindow::update(GuiController *controller)
{
	//! dont read behind the initial elements in case one was added
	
	if(controller->chan & GuiTrigger::CHANNEL_1)
	{
		u32 drcSize = drcElements.size();
		
		for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
		{
			drcElements[i]->update(controller);
		}
	}
	else
	{
		u32 tvSize = tvElements.size();
		
		for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
		{
			tvElements[i]->update(controller);
		}
	}
	
	if(controller->chanIdx >= 1 && controller->chanIdx <= 4 && controller->data.validPointer)
	{
		int wpadIdx = controller->chanIdx - 1;
		f32 posX = controller->data.x;
		f32 posY = controller->data.y;
		pointerImg[wpadIdx]->setPosition(posX, posY);
		pointerImg[wpadIdx]->setAngle(controller->data.pointerAngle);
		pointerValid[wpadIdx] = true;
	}
}

void MainWindow::drawDrc(CVideo *video)
{
	for(u32 i = 0; i < drcElements.size(); ++i)
	{
		drcElements[i]->draw(video);
	}
	
	for(int i = 0; i < 4; i++)
	{
		if(pointerValid[i])
		{
			pointerImg[i]->setAlpha(0.5f);
			pointerImg[i]->draw(video);
			pointerImg[i]->setAlpha(1.0f);
		}
	}
}

void MainWindow::drawTv(CVideo *video)
{
	for(u32 i = 0; i < tvElements.size(); ++i)
	{
		tvElements[i]->draw(video);
	}
	
	for(int i = 0; i < 4; i++)
	{
		if(pointerValid[i])
		{
			pointerImg[i]->draw(video);
			pointerValid[i] = false;
		}
	}
}

void MainWindow::SetupMainView()
{
	currentTvFrame = new GuiFrame(width, height);
	currentTvFrame->setEffect(EFFECT_FADE, 10, 255);
	currentTvFrame->setState(GuiElement::STATE_DISABLED);
	currentTvFrame->effectFinished.connect(this, &MainWindow::OnOpenEffectFinish);
	currentTvFrame->append(&splashImg);
	appendTv(currentTvFrame);
	
	currentDrcFrame = new GuiFrame(width, height);
	currentDrcFrame->setEffect(EFFECT_FADE, 10, 255);
	currentDrcFrame->setState(GuiElement::STATE_DISABLED);
	currentDrcFrame->effectFinished.connect(this, &MainWindow::OnOpenEffectFinish);
	currentDrcFrame->append(&bgParticleImg);
	
	if(!gInstallMiimakerAsked)
		SetBrowserWindow();
	
	SetDrcHeader();
	
	if(!gInstallMiimakerAsked && (folderList == NULL))
	{
		MessageBox * messageBox = new MessageBox(MessageBox::BT_OK, MessageBox::IT_ICONERROR, false);
		messageBox->setState(GuiElement::STATE_DISABLED);
		messageBox->setEffect(EFFECT_FADE, 10, 255);
		messageBox->setTitle("Error:");
		messageBox->setMessage1("No installable content found.");
		messageBox->effectFinished.connect(this, &MainWindow::OnOpenEffectFinish);
		messageBox->messageOkClicked.connect(this, &MainWindow::OnErrorMessageBoxClick);
		
		currentDrcFrame->append(messageBox);
	}
	
	appendDrc(currentDrcFrame);
}

void MainWindow::SetDrcHeader()
{
	titleText.setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	titleText.setFontSize(46);
	titleText.setPosition(0, 10);
	titleText.setBlurGlowColor(5.0f, glm::vec4(0.0, 0.0, 0.0f, 1.0f));
	
	versionText.setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	versionText.setFontSize(30);
	versionText.setPosition(-15, -40);
	versionText.setBlurGlowColor(5.0f, glm::vec4(0.0, 0.0, 0.0f, 1.0f));
	versionText.setAlignment(ALIGN_RIGHT | ALIGN_TOP);
	versionText.setTextf("%s (rev %s)",  WUP_GX2_VERSION, "1.0");
	
	headerFrame.setSize(titleImg.getWidth(), titleImg.getHeight());
	headerFrame.setPosition(0, 310);
	headerFrame.append(&titleImg);
	headerFrame.append(&titleText);
	headerFrame.append(&versionText);
	
	currentDrcFrame->append(&headerFrame);
}

void MainWindow::SetBrowserWindow()
{
	browserWindow = NULL;
	
	if(folderList == NULL)
	{
		folderList = new CFolderList();
		folderList->Get();
	}
	
	if(!folderList->GetCount())
	{
		delete folderList;
		folderList = NULL;
		return;
	}
	
	browserWindow = new BrowserWindow(920, height, folderList);
	browserWindow->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	browserWindow->setPosition(50, 0);
	browserWindow->installButtonClicked.connect(this, &MainWindow::OnInstallButtonClicked);
	browserWindow->setState(GuiElement::STATE_DISABLED);
	browserWindow->setEffect(EFFECT_FADE, 10, 255);
	browserWindow->effectFinished.connect(this, &MainWindow::OnOpenEffectFinish);
	currentDrcFrame->append(browserWindow);
}

void MainWindow::OnInstallButtonClicked(GuiElement *element)
{
	browserWindow->setEffect(EFFECT_FADE, -10, 255);
	browserWindow->setState(GuiElement::STATE_DISABLED);
	browserWindow->effectFinished.connect(this, &MainWindow::OnBrowserCloseEffectFinish);
	
	installWindow = new InstallWindow(folderList);
	installWindow->installWindowClosed.connect(this, &MainWindow::OnInstallWindowClosed);
}

void MainWindow::OnBrowserCloseEffectFinish(GuiElement *element)
{
	//! remove element from draw list and push to delete queue
	currentDrcFrame->remove(element);
	AsyncDeleter::pushForDelete(element);
}
void MainWindow::OnInstallWindowClosed(GuiElement *element)
{
	if(!gInstallMiimakerAsked)
	{
		SetBrowserWindow();
		currentDrcFrame->bringToFront(&headerFrame);
	}
	else
	{
//		Application::instance()->quit(0);
	}
}

void MainWindow::OnErrorMessageBoxClick(GuiElement *element, int ok)
{
//	Application::instance()->quit(0);
}

void MainWindow::OnMiiMakerInstallWindowClosed(GuiElement *element)
{
	gInstallMiimakerFinished = true;
//	Application::instance()->quit(0);
}

void MainWindow::OnOpenEffectFinish(GuiElement *element)
{
	//! once the menu is open reset its state and allow it to be "clicked/hold"
	element->effectFinished.disconnect(this);
	element->clearState(GuiElement::STATE_DISABLED);
	
	if(gInstallMiimakerAsked && folderList == NULL)
	{
		folderList = new CFolderList();
		folderList->GetFromArray();
		
		installWindow = new InstallWindow(folderList);
		installWindow->installWindowClosed.connect(this, &MainWindow::OnMiiMakerInstallWindowClosed);
	}
}

void MainWindow::OnCloseEffectFinish(GuiElement *element)
{
	//! remove element from draw list and push to delete queue
	remove(element);
	AsyncDeleter::pushForDelete(element);
}
