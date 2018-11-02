#include "error.h"

Error::Error(int number, const std::string& sentence, int level) throw()
     :m_number(number), m_sentence(sentence), m_level(level)
{}
const char* Error::what() const throw()
{
    return m_sentence.c_str();
}

int Error::number() const throw()
{
    return m_number;
}

int Error::level() const throw()
{
    return m_level;
}

QString Error::errorMessage() const throw()
{
    return TranslatedStrings::error + " " + QString::number(m_number) + ": " + QString(m_sentence.c_str());
}

void Error::display(QWidget* parent) const throw()
{
    QMessageBox::critical(parent, TranslatedStrings::error, errorMessage());
}

void Error::print() const throw()
{
    qFatal(errorMessage().toStdString().c_str());
}

Error::~Error() throw()
{}
