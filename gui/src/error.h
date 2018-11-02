#ifndef ERROR_H
#define ERROR_H


#include <exception>
#include <string>
#include <QMessageBox>
#include "globalconstants.h"

class Error: public std::exception
{
    /* This class stores data about runtime errors*/
public:
    Error(int number=0, const std::string& sentence="", int level=0) throw();
    virtual const char* what() const throw();
    int number() const throw();
    int level() const throw();
    QString errorMessage() const throw();
    void display(QWidget* parent) const throw();
    void print() const throw();
    virtual ~Error() throw();

private:
    int m_number;
    std::string m_sentence;
    int m_level;

};

#endif // ERROR_H
