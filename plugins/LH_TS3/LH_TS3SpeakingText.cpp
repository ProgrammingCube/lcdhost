#ifndef LH_TS3SPEAKINGTEXT_H
#define LH_TS3SPEAKINGTEXT_H

#include <QtGlobal>
#include <QTime>
#include <QFont>
#include <QFontMetrics>

#include "../LH_Text/LH_Text.h"
#include "LH_Qt_QString.h"

class LH_TS3SpeakingText : public LH_Text
{
public:
    const char *userInit()
    {
        if( const char *err = LH_Text::userInit() ) return err;
        //setup_text_->setTitle("Talking");
        setup_text_->setFlag(LH_FLAG_READONLY, true);
        setup_text_->setFlag(LH_FLAG_HIDDEN, true);
        setup_text_->setFlag(LH_FLAG_NOSAVE, true);
        setText("");
        setup_text_->setLink("=/Monitoring/3rdParty/TeamSpeak3/Talking");
        //setup_text_->refreshData();
        return 0;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "3rdParty/TeamSpeak 3",
            "TS3SpeakingText",
            "TS3 Speaking (Text)",
            -1, -1
        };

        if( classInfo.width == -1 )
        {
            QFont font;
            QFontMetrics fm( font );
            classInfo.height = fm.height();
            classInfo.width = fm.width("LCDHost");
        }

        return &classInfo;
    }
};

LH_PLUGIN_CLASS(LH_TS3SpeakingText)

#endif // LH_TS3SPEAKINGTEXT_H