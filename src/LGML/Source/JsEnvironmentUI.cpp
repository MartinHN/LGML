#include "JsEnvironmentUI.h"

JsEnvironmentUI::JsEnvironmentUI(JsEnvironment * _env) :env(_env) {
	env->addListener(this);
	loadFileB.setButtonText("Load");
	addAndMakeVisible(loadFileB);
	loadFileB.addListener(this);

	reloadB.setButtonText("Reload");
	addAndMakeVisible(reloadB);
	reloadB.addListener(this);

	openB.setButtonText("Show");
	addAndMakeVisible(openB);
	openB.addListener(this);

	watchT.setButtonText("autoWatch");
	watchT.setClickingTogglesState(true);
	addAndMakeVisible(watchT);
	watchT.addListener(this);

	logEnvB.setButtonText("Log");
	addAndMakeVisible(logEnvB);
	logEnvB.addListener(this);



    newJsFileLoaded(env->hasValidJsFile());
	addAndMakeVisible(validJsLed);

}

void JsEnvironmentUI::resized() {
	Rectangle<int> area = getLocalBounds().reduced(2);
	const int logEnvSize = 40;
	const int ledSize = 10;
	const int step = (area.getWidth() - logEnvSize - ledSize) / 4;
	buildLed(ledSize);
	validJsLed.setBounds(area.removeFromLeft(ledSize).reduced(0, (area.getHeight() - ledSize) / 2));
	loadFileB.setBounds(area.removeFromLeft(step).reduced(2));
	reloadB.setBounds(area.removeFromLeft(step).reduced(2));
	openB.setBounds(area.removeFromLeft(step).reduced(2));
	watchT.setBounds(area.removeFromLeft(step).reduced(2));
	logEnvB.setBounds(area.removeFromLeft(logEnvSize).reduced(2));

}

void JsEnvironmentUI::buildLed(int size) {
	Path circle;
	circle.addEllipse(Rectangle<float>(0, 0, (float)size, (float)size));
	validJsLed.setPath(circle);
}

void JsEnvironmentUI::newJsFileLoaded(bool s) {
  validJsLed.setFill(FillType((env->hasValidJsFile() && env->isInSyncWithLGML()) ? Colours::green :
                              (env->hasValidJsFile() ? Colours::orange :
                               Colours::red)));
}

void JsEnvironmentUI::buttonClicked(Button * b) {
	if (b == &loadFileB) {
		FileChooser myChooser("Please select the script you want to load...",
			File::getSpecialLocation(File::userHomeDirectory),
			"*.js");

		if (myChooser.browseForFileToOpen())
		{
			File script(myChooser.getResult());
			env->loadFile(script);
		}
	}
	else if (b == &openB) {
		env->showFile();
	}
	else if (b == &reloadB) {
		env->reloadFile();
	}
	else if (b == &logEnvB) {
		LOG(env->printAllNamespace());
	}
	else if (b == &watchT) {
		env->setAutoWatch(watchT.getToggleState());
	}
}
