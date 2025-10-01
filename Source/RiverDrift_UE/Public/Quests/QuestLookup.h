#pragma once

//#include "Quests/RDQuestLine.h"
#include "UI/DA_RDDialogueScene.h"
#include "QuestLookup.generated.h"


USTRUCT(BlueprintType)
struct RIVERDRIFT_UE_API FQuestLookup : public FTableRowBase
{
	GENERATED_BODY()

	FQuestLookup() {
		//UE_LOG(QuestLog, Log, TEXT("constructor called"))
		QuestID = FGuid::NewGuid();
	};

	//FQuestLookup(UDA_RDDialogueScene* Scene) {
	//	FQuestLookup();
	//	//DialogueScene = Scene;

	//};

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tiles")
	FGuid QuestID;


	//Used to set the reference to what other object is associated with this quest. 
	// When the player "interacts" with this object, the quest will progress
	// - Table: needs to be set to the landmarks data table or the dialogue quest lookup table
	// - Row: the row within that table that will identify the object
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FDataTableRowHandle OtherObjectRowHandle;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tiles")
	//TObjectPtr<UDA_RDDialogueScene> DialogueScene;


};



USTRUCT(BlueprintType)
struct RIVERDRIFT_UE_API FDialogueQuestLookup : public FQuestLookup
{
	GENERATED_BODY()

	FDialogueQuestLookup() {
		//UE_LOG(QuestLog, Log, TEXT("constructor called"))
		QuestID = FGuid::NewGuid();
	};

	FDialogueQuestLookup(UDA_RDDialogueScene* Scene) {
		FDialogueQuestLookup();
		DialogueScene = Scene;

	};

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tiles")
	TObjectPtr<UDA_RDDialogueScene> DialogueScene;

};
 