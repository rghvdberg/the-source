/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_120220767_INCLUDED
#define BINARYDATA_H_120220767_INCLUDED

namespace BinaryData
{
    extern const char*   delayReverse_png;
    const int            delayReverse_pngSize = 362;

    extern const char*   dottedNote_png;
    const int            dottedNote_pngSize = 229;

    extern const char*   lfoGain_png;
    const int            lfoGain_pngSize = 525;

    extern const char*   lfoSampleHold_png;
    const int            lfoSampleHold_pngSize = 203;

    extern const char*   lfoSineWave_png;
    const int            lfoSineWave_pngSize = 355;

    extern const char*   lfoSquareWave_png;
    const int            lfoSquareWave_pngSize = 158;

    extern const char*   oscWaveForms_png;
    const int            oscWaveForms_pngSize = 1154;

    extern const char*   recordCutoff_png;
    const int            recordCutoff_pngSize = 491;

    extern const char*   seqRandom_png;
    const int            seqRandom_pngSize = 335;

    extern const char*   seqSequential_png;
    const int            seqSequential_pngSize = 266;

    extern const char*   seqUpDown_png;
    const int            seqUpDown_pngSize = 300;

    extern const char*   synisterLogo_png;
    const int            synisterLogo_pngSize = 2844;

    extern const char*   tempoSync_png;
    const int            tempoSync_pngSize = 552;

    extern const char*   toggleOff_png;
    const int            toggleOff_pngSize = 465;

    extern const char*   toggleOn_png;
    const int            toggleOn_pngSize = 661;

    extern const char*   triplets_png;
    const int            triplets_pngSize = 464;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 16;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
