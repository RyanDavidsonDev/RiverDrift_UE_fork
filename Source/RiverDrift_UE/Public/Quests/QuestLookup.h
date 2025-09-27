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
 