// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    FBullCowCount Count;
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    Isograms = GetValidWords(Words);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver){
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to the bull cows mini game!"));
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    // PrintLine(TEXT("The amount of possible hidden words is:\n%i"), Words.Num());
    // PrintLine(TEXT("The amount of possible valid words is:\n%i"), GetValidWords(Words).Num());
    PrintLine(TEXT("The hiddenword is %s."), *HiddenWord); //Debug line
    PrintLine(TEXT("Guess the %i letter word!\nYou have %i lives."), HiddenWord.Len(),Lives);
    PrintLine(TEXT("Type in your guess.\nPress enter to continue..."));
    
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("You got all the bulls CONGRATULATIONS!!!"));
        EndGame();
        return;
    }

    if(HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("Sorry try guessing again!\nYou have %i lives remaining."), Lives);
        PrintLine(TEXT("The hidden word is %i chars long."), HiddenWord.Len());
        return;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("There are no repeating letters, \nguess again!"));
    }
    
    //Remove a life.
    PrintLine(TEXT("Lost a life."));
    --Lives;

    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!!!"));
        PrintLine(TEXT("The hidden word was: %s."), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows..."), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again you have %i lives."), Lives);

}
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 Index=0; Index < Word.Len()-1; Index++)
    {
        for(int32 Comparison=Index+1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }


    return true;
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if(Word.Len()>=4 && Word.Len()<=8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess)const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for(int32 NewIndex = 0;NewIndex < HiddenWord.Len(); NewIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[NewIndex])
            {
                Count.Cows++;
            }
        }

    }

    return Count;
}