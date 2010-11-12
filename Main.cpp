#include "QuestionFactory.h"
#include "QuestionGenreSwitcher.h"

#include <iostream>
#include <string>

int main()
{
    std::cout << "This program generates Questions and checks Answers!" << std::endl;

    int NumberOfCorrectQuestions = 0;
    int NumberOfWrongQuestions   = 0;
    int NumberOFQuestions        = 9;

    QuestionFactory*      questionFactory;
    QuestionGenreSwitcher questionGenreSwitcher;


    //std::cout << "Type 1 for Addition problems, 2 for Subtraction or 3 for crazy Random:" << std::endl;
    //std::string choice;
    //getline (std::cin,choice);


    for (int i = 0; i < NumberOFQuestions; i++)
    {

        //let's spice it up by calling random genre using random and polymorphism and pointer vectors
        questionFactory = questionGenreSwitcher.getFactory();

        //ask user a question
        std::cout << questionFactory->getQuestion() << std::endl;

        //get user response save to string
        std::string str;
        getline (std::cin,str);
        questionFactory->setPlayerAnswer(str);

        //check user answer with correct answer and print out whether correct or not.
        if (questionFactory->checkAnswer())
        {
            std::cout << "Correct" << std::endl;
            NumberOfCorrectQuestions++;
        }
        else
        {
            std::cout << "Wrong" << std::endl;
            NumberOfWrongQuestions++;
        }
    }

    //let's report how the user did
    std::cout << "You Answered " << NumberOfCorrectQuestions << " Correct Out of " << NumberOFQuestions << std::endl;
}
