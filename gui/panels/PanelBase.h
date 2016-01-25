
#pragma once

#include <map>

#include "JuceHeader.h"
#include "SynthParams.h"
#include "MouseOverKnob.h"

class PanelBase : public Component, protected Timer
{
public:
    PanelBase(SynthParams &p)
        : params(p) {
        startTimerHz(60);
    }

    ~PanelBase() {
        stopTimer();
    }

protected:
    void registerSlider(Slider *slider, Param *p) {
        sliderReg[slider] = p;
        if(p->hasLabels()) {
            slider->setName(p->getUIString());
        } else {
            slider->setName(p->name());
            slider->setTextValueSuffix(String(" ") + p->unit());
        }
        slider->setValue(p->getUI());
    }

    void registerSlider(MouseOverKnob *slider, Param *p) {
        registerSlider(static_cast<Slider*>(slider), p);
        slider->initTextBox();
    }

    void registerSaturn(MouseOverKnob *dest, Slider *source1, Slider *source2) {
        std::array<Slider*, 2> sources = {source1, source2};
        saturnReg[dest] = sources;
    }
    
    void updateDirtySliders() {
        for (auto s2p : sliderReg) {
            if (s2p.second->isUIDirty()) {
                s2p.first->setValue(s2p.second->getUI());
                if(s2p.second->hasLabels()) {
                    s2p.first->setName(s2p.second->getUIString());
                }
            }

        }
    }

    // Callback function in case there are any dirty saturn glows
    void updateDirtySaturns() {
        // iterate over all the registered saturn glows
        for (auto dest2saturn : saturnReg) {
            // find the mod source from the slider register
            for (int i = 0; i < 2; ++i) {
                auto modSource = sliderReg.find(dest2saturn.second[i]);
                
                //if the mod source is Dirty repaint
                if (modSource != sliderReg.end() && modSource->second->isUIDirty()) {
                    dest2saturn.first->repaint();
                }
                
            }
        }
    }
    
    bool handleSlider(Slider* sliderThatWasMoved) {
        auto it = sliderReg.find(sliderThatWasMoved);
        if (it != sliderReg.end()) {
            it->second->setUI(static_cast<float>(it->first->getValue()));
            if(it->second->hasLabels()) {
                it->first->setName(it->second->getUIString());
            }
            
            for (auto saturn : saturnReg) {
                for (int i = 0; i < 2; ++i) {
                    if (saturn.second[i] == sliderThatWasMoved) {
                        saturn.first->repaint();
                    }                    
                }
            }
            
            return true;
        } else {
            return false;
        }
    }

    virtual void timerCallback() override {
        updateDirtySaturns();
        updateDirtySliders();
    }

    std::map<Slider*, Param*> sliderReg;
    std::map<MouseOverKnob*, std::array<Slider*, 2>> saturnReg;
    SynthParams &params;
};
