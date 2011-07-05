/**
  \file     LH_QtCFInstance.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010-2011 Andy Bridges

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <QDebug>
#include "LH_QtCFInstance.h"
#include "cf_rule.h"


LH_QtCFInstance::LH_QtCFInstance() : LH_QtInstance()
{
    cf_initialized_ = false;
    return;
}

/**
  Conditional Formatting Routines

  to add conditional formatting to an object simply
  add source objects and target objects:

  \code
    add_cf_source(setup_text_);
    add_cf_target(setup_pencolor_);
    add_cf_target(setup_bgcolor_);
    add_cf_target(setup_font_);
  \endcode

  Source objects are watched for chagnes and their
  new values tested against the specified rules.

  Target objects are changed when rules are met.
  */

int LH_QtCFInstance::notify(int n,void* p)
{
    Q_UNUSED(p);
    if( n&LH_NOTE_SECOND )
        cf_apply_rules();
    return (watching_non_setup_item_ && setup_cf_enabled_->value()? LH_NOTE_SECOND : 0) | LH_QtInstance::notify(n, p);
}

void LH_QtCFInstance::cf_initialize()
{
    if (cf_initialized_)
        return;
    else
    {
        int LH_FLAG_UI = LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK ;

        cf_initialized_ = true;
        cf_rule_editing_ = None;
        watching_non_setup_item_ = false;

        new LH_Qt_QString(this,tr("~CF-Area-Rule"),"<hr>",LH_FLAG_LAST | LH_FLAG_UI,lh_type_string_html );
        setup_cf_enabled_ = new LH_Qt_bool(this, "^Enable Conditional Formatting", false, LH_FLAG_LAST | LH_FLAG_AUTORENDER);
        new LH_Qt_QString(this,tr("^comment"),"<span style='color:grey'>(Conditional Formatting is still experimental)</span>",LH_FLAG_LAST | LH_FLAG_UI,lh_type_string_html );

        setup_cf_state_ = new LH_Qt_QString(this, "State", "", LH_FLAG_NOSAVE | LH_FLAG_READONLY | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_copy_ = new LH_Qt_QString(this, "^Copy Conditions", "Copy",  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_button);
        setup_cf_paste_ = new LH_Qt_QString(this, "^Paste Conditions", "Paste",  LH_FLAG_UI | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_button);

        setup_cf_rules_ = new LH_Qt_QStringList(this, "Conditions", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_integer_listbox);

        setup_cf_move_up_= new LH_Qt_QString(this, "^Move Condition Up", "Move Up",  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_READONLY, lh_type_string_button);
        setup_cf_move_down_= new LH_Qt_QString(this, "^Move Condition Down", "Move Down",  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_READONLY, lh_type_string_button);

        setup_cf_new_ = new LH_Qt_QString(this, "^New Condition", "New",  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_button);
        setup_cf_delete_ = new LH_Qt_QString(this, "^Delete Condition", "Delete", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_READONLY, lh_type_string_button);

        setup_cf_source_ = new LH_Qt_QStringList(this, "Source", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_source_mode_ = new LH_Qt_QStringList(this, "Value", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_test_ = new LH_Qt_QStringList(this, "Condition",
                                                    QStringList()
                                                    << "Equals"       << "Does not equal"
                                                    << "Less than"    << "Less than or equal"
                                                    << "Greater than" << "Greater than or equal"
                                                    << "Between"      << "Is not between"
                                                    << "Begins with"  << "Does not begin with"
                                                    << "Ends with"    << "Does not end with"
                                                    << "Contains"     << "Does not contain"
                                                    , LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_testValue1_ = new LH_Qt_QString(this, "^Value 1", "", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_testValue2_ = new LH_Qt_QString(this, "^Value 2", "", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_target_ = new LH_Qt_QStringList(this, "Target", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_newValue_Color_ = new LH_Qt_QColor(this, "^New Value - Color",QColor(0,0,0),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_Font_  = new LH_Qt_QFont(this, "^New Value - Font",QFont("Arial",10),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_String_= new LH_Qt_QString(this, "^New Value - String","",LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_File_  = new LH_Qt_QFileInfo(this,"^New Value - File",QFileInfo(""),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_save_   = new LH_Qt_QString(this, "^Save Condition", "Save", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_button);
        setup_cf_cancel_ = new LH_Qt_QString(this, "^Cancel Condition Edit", "Cancel", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_button);

        setup_cf_XML_ = new LH_Qt_QTextEdit(this, "Conditions XML", "<rules/>", LH_FLAG_LAST | LH_FLAG_HIDEVALUE);

        connect(setup_cf_enabled_, SIGNAL(changed()), this, SLOT(cf_enabled_changed()));
        connect(setup_cf_source_, SIGNAL(changed()), this, SLOT(cf_source_changed()));
        connect(setup_cf_source_, SIGNAL(set()), this, SLOT(cf_source_changed()));
        connect(setup_cf_target_, SIGNAL(changed()), this, SLOT(cf_target_changed()));
        connect(setup_cf_target_, SIGNAL(set()), this, SLOT(cf_target_changed()));
        connect(setup_cf_test_, SIGNAL(changed()), this, SLOT(cf_condition_changed()));
        connect(setup_cf_test_, SIGNAL(set()), this, SLOT(cf_condition_changed()));
        connect(setup_cf_save_, SIGNAL(changed()), this, SLOT(cf_save_rule()));
        connect(setup_cf_cancel_, SIGNAL(changed()), this, SLOT(cf_cancel_edit_rule()));
        connect(setup_cf_XML_, SIGNAL(changed()), this, SLOT(cf_XML_changed()));
        connect(setup_cf_rules_, SIGNAL(changed()), this, SLOT(cf_rules_changed()));
        connect(setup_cf_delete_, SIGNAL(changed()), this, SLOT(cf_delete_rule()));
        connect(setup_cf_new_, SIGNAL(changed()), this, SLOT(cf_new_rule()));

        connect(setup_cf_copy_, SIGNAL(changed()), this, SLOT(cf_copy_rules()));
        connect(setup_cf_paste_, SIGNAL(changed()), this, SLOT(cf_paste_rules()));
        connect(setup_cf_move_up_, SIGNAL(changed()), this, SLOT(cf_move_rule_up()));
        connect(setup_cf_move_down_, SIGNAL(changed()), this, SLOT(cf_move_rule_down()));

        connect(setup_cf_state_, SIGNAL(changed()), this, SLOT(cf_state_value_updated()));
        connect(setup_cf_state_, SIGNAL(set()), this, SLOT(cf_state_value_updated()));

        add_cf_source(setup_cf_state_);
        add_cf_target(setup_cf_state_);       
    }
}

void LH_QtCFInstance::add_cf_source(LH_QtSetupItem *si)
{
    add_cf_source(si->name(), si);
}

void LH_QtCFInstance::add_cf_source(QString name)
{
    add_cf_source(name, NULL);
}

void LH_QtCFInstance::add_cf_source(QString name, LH_QtSetupItem* si)
{
    cf_initialize();
    if(sources_.contains(name))
        return;

    if(sources_.count()==0)
        setup_cf_source_->list().append(name);
    else
        setup_cf_source_->list().insert(sources_.count()-1, name);

    sources_.insert(new cf_source(this, name, si));
    setup_cf_source_->refreshList();

    if(si==NULL)
        watching_non_setup_item_ = true;
    else
    {
        sources_[name]->setValue();
        if(si!=setup_cf_state_)
        {
            connect(si, SIGNAL(changed()), this, SLOT(cf_apply_rules()));
            connect(si, SIGNAL(set()), this, SLOT(cf_apply_rules()));
        }
    }
}

void LH_QtCFInstance::add_cf_target(LH_QtSetupItem *si)
{
    cf_initialize();
    if(targets_.length()==0)
    {
        targets_.append(si);
        setup_cf_target_->list().append(si->name());
    }
    else
    {
        int i = targets_.length()-1;
        targets_.insert(i, si);
        setup_cf_target_->list().insert(i, si->name());
    }
    setup_cf_target_->refreshList();
}

void LH_QtCFInstance::cf_enabled_changed()
{

    setup_cf_rules_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());

    setup_cf_new_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    setup_cf_delete_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    setup_cf_move_up_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    setup_cf_move_down_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    setup_cf_copy_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    setup_cf_paste_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());

    setup_cf_state_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());

    cf_set_edit_controls_visibility();

    callback(lh_cb_notify, NULL);
}

void LH_QtCFInstance::cf_set_edit_controls_visibility(cf_rule_edit_mode editMode)
{
    if(editMode!=Default)
    cf_rule_editing_ = editMode;

    setup_cf_source_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_test_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_testValue1_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_target_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);

    setup_cf_save_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_cancel_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);

    setup_cf_delete_->setFlag(LH_FLAG_READONLY, cf_rule_editing_!=Existing);
    setup_cf_move_up_->setFlag(LH_FLAG_READONLY, cf_rule_editing_!=Existing);
    setup_cf_move_down_->setFlag(LH_FLAG_READONLY, cf_rule_editing_!=Existing);

    cf_source_changed();
    cf_condition_changed();
    cf_target_changed();
}

void LH_QtCFInstance::cf_source_changed()
{
    if (setup_cf_source_->value()>=0)
    {
        setup_cf_source_mode_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None );

        setup_cf_source_mode_->list().clear();
        setup_cf_source_mode_->list().append(sources_[setup_cf_source_->valueText()]->getModes());
        setup_cf_source_mode_->refreshList();
    }
}

void LH_QtCFInstance::cf_target_changed()
{
    if (setup_cf_target_->value()>=0 && setup_cf_target_->value()<targets_.length())
    {
        setup_cf_newValue_Color_->setFlag( LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_Color_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_Font_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_Font_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_String_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_String_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_File_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_File_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
    }
}

void LH_QtCFInstance::cf_condition_changed()
{
    QString conditionText = setup_cf_test_->valueText();
    setup_cf_testValue2_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || !conditionText.contains("etween") || cf_rule_editing_==None);
}

void LH_QtCFInstance::cf_cancel_edit_rule()
{
    setup_cf_rules_->setValue(-1);
    cf_set_edit_controls_visibility(None);
}

/*
QDomElement LH_QtCFInstance::createNode(QDomDocument doc, QString tagName, QString nodeValue, QString attributeName, QString attributeValue)
{
    QDomElement e = doc.createElement(tagName);
    if(attributeName!="")
        e.setAttribute(attributeName, attributeValue);
    if(nodeValue!="")
        e.appendChild(doc.createTextNode(nodeValue));
    return e;
}
*/

void LH_QtCFInstance::cf_XML_changed()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    int selIndex = setup_cf_rules_->value();
    if(cf_rule_editing_!=Existing) selIndex = -1;
    setup_cf_rules_->list().clear();
    for(uint i=0; i<root.childNodes().length(); i++)
    {
        cf_rule rule(root.childNodes().at(i));
        setup_cf_rules_->list().append(rule.description());
    }
    setup_cf_rules_->refreshList();    
    setup_cf_rules_->setValue(selIndex);
    cf_apply_rules();
}

void LH_QtCFInstance::cf_rules_changed()
{
    cf_source_changed();
    cf_set_edit_controls_visibility(Existing);

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    bool validSel = setup_cf_rules_->value()>=0 && setup_cf_rules_->value() < (int)root.childNodes().length();
    if(validSel)
    {
        cf_rule rule(root.childNodes().at(setup_cf_rules_->value()));

        rule.conditions[0]->edit(this);
        rule.actions[0]->edit(this, targets_);
    }

    setup_cf_delete_->setFlag(LH_FLAG_READONLY, !validSel);
    setup_cf_move_up_->setFlag(LH_FLAG_READONLY, !validSel);
    setup_cf_move_down_->setFlag(LH_FLAG_READONLY, !validSel);
}

void LH_QtCFInstance::cf_delete_rule()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());

    QDomNodeList rules = doc.firstChild().childNodes();
    if( setup_cf_rules_->value() >= 0 && setup_cf_rules_->value() < (int)rules.length() )
    {
        QDomNode n = rules.at(setup_cf_rules_->value());
        doc.firstChild().removeChild(n);
        setup_cf_XML_->setValue(doc.toString());
        cf_XML_changed();
        cf_rules_changed();
    }
}

void LH_QtCFInstance::cf_new_rule()
{
    setup_cf_rules_->setValue(-1);
    cf_set_edit_controls_visibility(New);

    setup_cf_source_->setValue(0);
    setup_cf_test_->setValue(0);
    setup_cf_testValue1_->setValue("");
    setup_cf_testValue2_->setValue("");
    setup_cf_target_->setValue(0);
}

void LH_QtCFInstance::cf_state_value_updated()
{
    if (QObject::sender()!=NULL)
    {
        QString senderName = ((LH_QtSetupItem*)QObject::sender())->name();
        if(sources_.contains(senderName))
            sources_[senderName]->setValue();
    }
}

void LH_QtCFInstance::cf_apply_rules(bool allowRender)
{
    if(!setup_cf_enabled_->value())
        return;

    if (QObject::sender()!=NULL)
    {
        QString senderName = ((LH_QtSetupItem*)QObject::sender())->name();
        if(sources_.contains(senderName))
            sources_[senderName]->setValue();
    }

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    bool doRender = false;

    for(uint i=0; i<root.childNodes().length(); i++)
        doRender |= cf_rule(root.childNodes().at(i)).apply(this, sources_, targets_);

    if(doRender && allowRender) this->requestRender();
}

void LH_QtCFInstance::cf_copy_rules()
{
    QFile file(QString("%1cf_cache.xml").arg(state()->dir_binaries));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    else
        file.write(setup_cf_XML_->value().toAscii());
    /*QClipboard *clipboard = QApplication::clipboard();
    QString newText = setup_cf_XML_->value();
    qDebug() << newText;
    clipboard->clear();*/
}

void LH_QtCFInstance::cf_paste_rules()
{
    QString clip_text;
    QFile file(QString("%1cf_cache.xml").arg(state()->dir_binaries));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    else
        clip_text = QString(file.readAll().data());


    //const QClipboard *clipboard = QApplication::clipboard();
    //const QMimeData *mimeData = clipboard->mimeData();
    //if(!mimeData->hasText())
    //    return;
    //QString clip_text = mimeData->text();

    QDomDocument doc_clip("");
    if(doc_clip.setContent(clip_text))
    {
        QDomElement root_clip = doc_clip.firstChild().toElement();

        QDomDocument doc_curr("");
        doc_curr.setContent(setup_cf_XML_->value());
        QDomElement root_curr = doc_curr.firstChild().toElement();

        QDomNodeList rules = root_clip.elementsByTagName("rule");
        while(rules.length()!=0)
            root_curr.appendChild(rules.at(0));

        setup_cf_XML_->setValue(doc_curr.toString());
        cf_XML_changed();
    }
}

void LH_QtCFInstance::cf_move_rule_up()
{
    int i = setup_cf_rules_->value();

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    if(i>=1 && i < (int)root.childNodes().length())
    {
        root.insertBefore(root.childNodes().at(i),root.childNodes().at(i-1));
        setup_cf_rules_->setValue(i-1);
    }

    setup_cf_XML_->setValue(doc.toString());
    cf_XML_changed();
}

void LH_QtCFInstance::cf_move_rule_down()
{
    int i = setup_cf_rules_->value();

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    if(i>=0 && i < (int)root.childNodes().length()-1)
    {
        root.insertAfter(root.childNodes().at(i),root.childNodes().at(i+1));
        setup_cf_rules_->setValue(i+1);
    }

    setup_cf_XML_->setValue(doc.toString());
    cf_XML_changed();
}

void LH_QtCFInstance::cf_source_notify(QString name, QString value, int index, int count)
{
    sources_[name]->setValue(value, index);
    if(index==count-1)
        cf_apply_rules(false);    
}

void LH_QtCFInstance::cf_save_rule()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());

    /*QDomElement ruleNode = createNode(doc, "rule");
    QDomElement condsNode = createNode(doc, "conditions");
    { // Condition Node
        QDomElement condNode = createNode(doc, "condition", "", "test", setup_cf_test_->valueText());
        QDomElement srcNode = createNode(doc, "source", setup_cf_source_->valueText());
        if(!setup_cf_source_mode_->hasFlag(LH_FLAG_HIDDEN))
            srcNode.setAttribute("mode",setup_cf_source_mode_->valueText());
        condNode.appendChild(srcNode);

        condNode.appendChild(createNode(doc, "value", setup_cf_testValue1_->value(), "id", "1"));
        if(!setup_cf_testValue2_->hasFlag(LH_FLAG_HIDDEN))
            condNode.appendChild(createNode(doc, "value", setup_cf_testValue2_->value(), "id", "2"));

        condsNode.appendChild(condNode);
    }
    ruleNode.appendChild(condsNode);


    QDomElement actsNode = createNode(doc, "actions");
    { // Action Node
        QDomElement actNode = createNode(doc, "action", "", "type", "property");
        actNode.appendChild(createNode(doc, "target", setup_cf_target_->valueText()));
        actNode.appendChild(createNode(doc, "value", getTargetValue(setup_cf_target_->value())));
        actsNode.appendChild(actNode);
    }
    ruleNode.appendChild(actsNode);
    */

    QDomNode ruleNode = cf_rule(this).toXmlNode(doc);

    if(setup_cf_rules_->value()==-1)
        doc.firstChild().appendChild(ruleNode);
    else
        doc.firstChild().replaceChild(ruleNode, doc.firstChild().childNodes().at(setup_cf_rules_->value()));

    setup_cf_XML_->setValue(doc.toString());
    cf_XML_changed();
}

/*QString LH_QtCFInstance::getTargetValue(int targetId)
{
    Q_ASSERT(false);
    switch(targets_[targetId]->type())
    {
    case lh_type_integer_color:
        return QString::number(setup_cf_newValue_Color_->value().rgba(), 16).toUpper();
    case lh_type_string_font:
        return setup_cf_newValue_Font_->value().toString();
    case lh_type_string:
        return setup_cf_newValue_String_->value();
    case lh_type_string_filename:
        return setup_cf_newValue_File_->value().filePath();
    default:
        return "";
    }
}*/
