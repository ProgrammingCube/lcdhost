#include "LH_TS3MuteImage.h"

LH_PLUGIN_CLASS(LH_TS3MuteImage)

bool LH_TS3MuteImage::loadPlaceholderImage(QImage *img)
{
    QString source = setup_mute_source_->valueText();
    QString status = setup_mute_status_->valueText();
    return img->load(QString(":/images/%1%2.png")
                .arg(status=="N/A"?"empty":(source=="Microphone"?"microphone":"sound"))
                .arg(status=="None"?"-disabled":(status=="Muted"?"-mute":""))
                );
}

const char *LH_TS3MuteImage::userInit()
{
    if( const char *err = LH_QImage::userInit() ) return err;

    //setup_show_placeholder_->setTitle("Use default images");

    setup_mute_source_ = new LH_Qt_QStringList(this, "Mute Source", QStringList() << "Speakers" << "Microphone", LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);

    setup_mute_status_ = new LH_Qt_QStringList(this, "Mute Status", QStringList() << "N/A" << "None" << "Muted" << "Active", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE_DATA | LH_FLAG_NOSAVE_LINK | LH_FLAG_AUTORENDER);
    setup_mute_status_->setLink("=/3rdParty/TeamSpeak 3/Speaker Status");

    connect(setup_mute_source_, SIGNAL(changed()), this, SLOT(changeSource()));

    add_cf_target(setup_image_file_);
    add_cf_source(setup_mute_status_);

    return 0;
}

lh_class *LH_TS3MuteImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/TeamSpeak 3",
        "TS3MuteImage",
        "TS3 Mute Status (Image)",
        16,16,
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };
    return &classInfo;
}

void LH_TS3MuteImage::changeSource()
{
    if(setup_mute_source_->valueText()=="Speakers")
        setup_mute_status_->setLink("=/3rdParty/TeamSpeak 3/Speaker Status");
    if(setup_mute_source_->valueText()=="Microphone")
        setup_mute_status_->setLink("=/3rdParty/TeamSpeak 3/Microphone Status");
}
