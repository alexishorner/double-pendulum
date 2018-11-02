#include <iostream>
#include <fstream>
#include <string>
#include <QStringList>
#include <stdlib.h>
#include "..\..\..\gui\src\doublependulum.h"
#include "..\..\..\gui\src\globalconstants.h"
#include "..\..\..\gui\src\error.h"

unsigned const argumentsPerPendulum(10);
unsigned const physicalParameters(argumentsPerPendulum-1);
unsigned const additionalArguments(4);
unsigned const maxNumberOfPendula(1000);
unsigned const linesPerBlock(1e4);


std::string lineOfData(DoublePendulum* pendulum)
{
    std::string line("\"" + pendulum->name().toStdString() + "\"" + ",");
    for(int j(0); j < Parameter::count; j++)
    {
        line.append(QString::number(pendulum->parameter(j)).toStdString()); //replaced std::to_string, because it may have a bug in MinGW
        if(j < Parameter::count-1)
        {
            line.append(",");
        }
    }
    //std::cerr << line << ", ";
    line.append("\n");
    return line;
}


int main(int argc, char* argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error(4, "missing argument: \"path\". Cannot read parameter file");
        }
        std::ifstream argumentsFile(argv[1]);
        std::vector<std::string> args;
        if(argumentsFile.good())
        {
            std::string line;
            std::string fileLines;
            while(std::getline(argumentsFile, line))
            {
                fileLines.append(line);
            }
            argumentsFile.close();
            QStringList qargs(QString::fromStdString(fileLines).split(","));
            args.resize(qargs.size());
            for(int i(0); i < qargs.size(); i++)
            {
                args.at(i) = qargs.at(i).toStdString();
            }
        }
        else
        {
            throw Error(5, "could not open the parameter file");
        }

        if(args.size() < argumentsPerPendulum + additionalArguments)
        {
            throw Error(0, std::to_string(argumentsPerPendulum + additionalArguments) + " parameters expected (got " + std::to_string(args.size()) + ")");
        }
        else if(args.size() > maxNumberOfPendula*argumentsPerPendulum+additionalArguments)
        {
            throw Error(1, "maximum number of pendula exceeded");
        }
        else if((args.size()-additionalArguments)/argumentsPerPendulum == int(round((args.size()-additionalArguments)/double(argumentsPerPendulum))))
        {
            double dt(atof(args[2].c_str()));

            if(fabs(dt) < 1.0e-9)
            {
                throw Error(6, "dt must at least be equal to 1e-9");
            }
            else
            {
                std::array<DoublePendulum*, maxNumberOfPendula> pendula;
                unsigned int numberOfPendula(round((args.size()-additionalArguments)/double(argumentsPerPendulum)));
                std::array<unsigned short, ArraySize::currentIndex> currentIndex;
                std::array<unsigned short, ArraySize::currentUnit> currentUnit;
                for(unsigned i(0); i < numberOfPendula; i++)
                {
                    for(unsigned j(0); j < ArraySize::currentIndex; j++)
                    {
                        currentIndex.at(j) = 0;
                    }

                    for(unsigned j(0); j < ArraySize::currentUnit; j++)
                    {
                        currentUnit.at(j) = 0;
                    }

                    QString name(QString::fromStdString(args.at(additionalArguments+argumentsPerPendulum*i)));

                    pendula.at(i) = new DoublePendulum(currentIndex, currentUnit, name);
                    pendula.at(i)->setIntegrationMethod(3); //Modified RK4
                    std::array<double, physicalParameters> values;
                    for(unsigned j(0); j < physicalParameters; j++)
                    {
                        values.at(j) = atof(args.at(additionalArguments+argumentsPerPendulum*i+j+1).c_str());
                    }


                    pendula.at(i)->changeAngle(values[0]);
                    currentIndex[Index::angle] = 1;
                    pendula.at(i)->changeAngle(values[1]);
                    currentIndex[Index::angle] = 0;

                    pendula.at(i)->changeAngularVelocity(values[2]);
                    currentIndex[Index::angVel] = 1;
                    pendula.at(i)->changeAngularVelocity(values[3]);
                    currentIndex[Index::angVel] = 0;

                    //std::cout << " "; //fixes a bug for no apparent reason

                    pendula.at(i)->changeRadius(values[4]);
                    currentIndex[Index::radius] = 1;
                    //std::cerr << pendula.at(i)->r1() << "r, " << values[4] << "i, ";
                    pendula.at(i)->changeRadius(values[5]);
                    //std::cerr << pendula.at(i)->r2() << "r, " << values[5] << "i, ";
                    currentIndex[Index::radius] = 0;

                    pendula.at(i)->changeMass(values[6]);
                    currentIndex[Index::mass] = 1;
                    pendula.at(i)->changeMass(values[7]);
                    currentIndex[Index::mass] = 0;

                    pendula.at(i)->changeGravity(values[8]);

                    pendula.at(i)->changeDt(dt);


                    //std::cerr << "pendulum " << i+1 << ", " << pendula.at(i)->posX1() << ", " << pendula.at(i)->posY1() << ", " << pendula.at(i)->posX2() << ", " << pendula.at(i)->posY2() << ", " << pendula.at(i)->ang1() << ", " << pendula.at(i)->ang2() << ", " << pendula.at(i)->angVel1() << ", " << pendula.at(i)->angVel2() << ", " << pendula.at(i)->v1() << ", " << pendula.at(i)->v2() << ", " << pendula.at(i)->angAcc1() << ", " << pendula.at(i)->angAcc2() << ", " << pendula.at(i)->r1() << ", " << pendula.at(i)->r2() << ", " << pendula.at(i)->m1() << ", " << pendula.at(i)->m2() << ", " << pendula.at(i)->potEnergy() << ", " << pendula.at(i)->kinEnergy() << ", " << pendula.at(i)->totEnergy() << ", " << pendula.at(i)->lagrangian() << ", ";
                }
                //std::cerr << std::endl;

                std::string dataPath(args[0].c_str());
                std::ofstream dataStream;
                try
                {
                    dataStream.open(dataPath);
                    if(!dataStream.is_open())
                    {
                        throw Error(3, "the file could not be opened.");
                    }
                    else
                    {
                        dataStream << "#" << "\"" << "Name" << "\"" << ",";
                        for(int i(0); i < Parameter::namesList.count(); i++)
                        {
                            dataStream << Parameter::namesList.at(i).toStdString();
                            if(i < Parameter::namesList.count()-1)
                            {
                                dataStream << ",";
                            }
                        }
                        dataStream << std::endl;
                    }
                }
                catch(Error const& e)
                {
                    throw;
                }




                std::array<std::string, linesPerBlock+maxNumberOfPendula> outputData;
                unsigned long lineNumber(0);
                double outputsPerSecond(atof(args[1].c_str()));
                double timeToSimulate(atof(args[3].c_str()));
                unsigned long numberOfOutputs(0);
                double progress(0);
                double lastProgress(0);

                std::cout << "\r\t" << "progress: " << progress << "%" << std::flush;
                for(unsigned i(0); i < numberOfPendula; i++)
                {
                outputData[lineNumber] = lineOfData(pendula.at(i));
                lineNumber++;
                }
                numberOfOutputs++;
                while(pendula[0]->time() < timeToSimulate)
                {
                    for(unsigned i(0); i < numberOfPendula; i++)
                    {
                        pendula.at(i)->moveOneStep();
                    }


                    if(pendula[0]->time()/numberOfOutputs > 1.0/outputsPerSecond)
                    {
                        for(unsigned i(0); i < numberOfPendula; i++)
                        {
                            outputData[lineNumber] = lineOfData(pendula.at(i));
                            lineNumber++;
                        }
                        //std::cerr << std::flush;
                        numberOfOutputs++;
                    }


                    double simulationTime(pendula[0]->time());
                    if(lineNumber >= linesPerBlock || simulationTime >= timeToSimulate)
                    {
                        for(std::array<std::string, linesPerBlock+maxNumberOfPendula>::iterator it = outputData.begin(); it != outputData.begin()+lineNumber; it++)
                        {
                            dataStream.write(it->data(), it->size());
                        }
                        lineNumber = 0;
                    }


                    progress = simulationTime/timeToSimulate*100.0;
                    if(progress - lastProgress >= 1)
                    {
                        lastProgress = progress;
                        std::cout << "\r\t" << "progress: " << int(round(progress)) << "%" << std::flush;
                    }
                }
                std::cout << "\r\t" << "simulation succeeded" << std::endl;




                for(unsigned i(0); i < numberOfPendula; i++)
                {
                    delete pendula.at(i);
                    pendula.at(i) = nullptr;
                }
                return 0;
            }
        }
        else
        {
            throw Error(2, "invalid number of parameters");
        }
    }
    catch(Error const& e)
    {
        std::cerr << e.errorMessage().toStdString() << std::endl;
        return 1;
    }
}
