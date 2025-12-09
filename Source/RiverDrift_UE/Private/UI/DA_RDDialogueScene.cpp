// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DA_RDDialogueScene.h"
#include "Quests/QuestLookup.h"
#include "Quests/RDQuestManagerSubsystem.h"


FRDDialogueResponseOption::FRDDialogueResponseOption()
{
	//UE_LOG(LogTemp, Log, TEXT("dialogue response option default constr called"))
	//ButtonText = FText::FromString("[continue]");
}

FRDDialogueResponseOption::FRDDialogueResponseOption(FText _ButtonText)
{
	ButtonText = _ButtonText;
}

FRDDialogueSlide::FRDDialogueSlide() 
{
	UE_LOG(LogTemp, Log, TEXT("dialogue slide default constr called"))
		LeftResponse = FRDDialogueResponseOption();
	RightResponse = FRDDialogueResponseOption(FText::FromString("[continue]"));

}

UDA_RDDialogueScene::UDA_RDDialogueScene() {
	//TablePath = 

}
//
//UDA_RDDialogueScene::UDA_RDDialogueScene(const UDA_RDDialogueScene& other) {
//	this->QuestLookupRow = other.QuestLookupRow;
//	this->QuestLookupDataTable = other.QuestLookupDataTable;
//	this->Slides = other.Slides;
//
//
//}

#if WITH_EDITOR
void UDA_RDDialogueScene::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject)) // avoid running for CDO
	{
		//CTD CTODO: fix this up so that it automatically updates the data table in editor but not on package
		//RegisterDataTable();
	}
}
#endif
void UDA_RDDialogueScene::PostLoad()
{
	Super::PostLoad();
	//RegisterDataTable();
}

void UDA_RDDialogueScene::RegisterDataTable()
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *TablePath);

	if (!DataTable) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load data table at %s"), *TablePath);
		return;
	}

	//URDQuestManagerSubsystem::

	const FName RowName = FName(*InternalName);
	//FDialogueQuestLookup* ExistingRow = DataTable->FindRow<FDialogueQuestLookup>(RowName, TEXT("RegisterDataTable()"));
	if (FDialogueQuestLookup* ExistingRow = DataTable->FindRow<FDialogueQuestLookup>(RowName, TEXT("RegisterDataTable()"))) {
		QuestLookupRow = ExistingRow;
		
		UE_LOG(LogTemp, Log, TEXT("row already exists %s"), *RowName.ToString());
		return ;
	}
	UE_LOG(LogTemp, Error, TEXT("ERROR: dialogue scene was not able to find a corresponding row, scene's object name is %s"), *InternalName );

#if WITH_EDITOR
	FDialogueQuestLookup NewRow;
	NewRow.DialogueScene = this;
	QuestLookupRow = NewRow;

	//FDialogueQuestLookup NewRow;
	//NewRow->DialogueScene = this;
	//QuestLookupRow = FDialogueQuestLookup();
	//QuestLookupRow = NewObject<FDialogueQuestLookup>();
	QuestLookupRow->DialogueScene = this;
	//QuestLookupRow = NewRow;

	DataTable->AddRow(RowName, *QuestLookupRow);
	UE_LOG(LogTemp, Log, TEXT("Registered asset '%s' in data table '%s'"), *InternalName, *TablePath);

	
		DataTable->Modify();
	DataTable->MarkPackageDirty();
#endif
	
}

void UDA_RDDialogueScene::CloseDialogueScene()
{

	//URDQuestManagerSubsystem

	//QuestLookupRow->
	//

	//UE_LOG(LogTemp, Log, TEXT("Dialogue scene finished"))


	//UDataTable* DataTable = LoadObject<UDataTable>(this, *TablePath);

	//if (!DataTable) {
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load data table at %s"), *TablePath);
	//	return;
	//}

	//const FName RowName = FName(*GetName());
	//FDialogueQuestLookup* Row = DataTable->FindRow<FDialogueQuestLookup>(RowName, TEXT("CloseDataTable()"));
	////QuestLookupRow = Row;
	//if (Row) {

	//	UE_LOG(LogTemp, Log, TEXT("row already exists %s"), *RowName.ToString());
	//	//return;
	//}
	//else {

	//	UE_LOG(LogTemp, Log, TEXT("row DOESN'T already exist "));
	//}

	////FDialogueQuestLookup NewRow;
	//NewRow->DialogueScene = this;
	//QuestLookupRow = NewRow;

	//DataTable->AddRow(RowName, *NewRow);
	//UE_LOG(LogTemp, Log, TEXT("Registered asset '%s' in data table '%s'"), *GetName(), *TablePath);



	//if (!QuestLookupRow || !QuestLookupRow->QuestID.IsValid()) {
	//	UE_LOG(LogTemp, Log, TEXT("quest lookup not valid, registering"))

	//	RegisterDataTable();
	//}
	//if(!Row || !Row->QuestID.IsValid()) {
	//	UE_LOG(LogTemp, Error, TEXT("quest lookup STILL not valid, exiting"))
	//		return;
	//		//RegisterDataTable();
	//}

	if (QuestLookupRow == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("ERROR: quest lookup row not valid"))
			return;

	}
	else {
		GetWorld()->GetSubsystem<URDQuestManagerSubsystem>()->CheckProgression(
			EConditionType::CE_Dialogue, QuestLookupRow->QuestID);

	}


}


//UDA_RDDialogueScene::UDA_RDDialogueScene()
//{
//	QuestLookupDataTable = GetAssetByObjectPath<
//}
