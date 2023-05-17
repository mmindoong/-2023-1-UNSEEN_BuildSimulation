#pragma once

#include "Gate.generated.h"

USTRUCT(BlueprintType)
struct FGate
{
	GENERATED_BODY()
public:
	FORCEINLINE FGate();
	explicit FORCEINLINE FGate(bool bStartClosed);

	FORCEINLINE void Open() { bGateOpen = true; }

	FORCEINLINE void Close() { bGateOpen = false; }

	FORCEINLINE void Toggle() { bGateOpen = !bGateOpen; }

	FORCEINLINE bool IsOpen() const { return bGateOpen; } 
private:
	UPROPERTY(VisibleAnywhere)
	bool bGateOpen;
};

	
FORCEINLINE FGate::FGate() : bGateOpen(false) {}
FORCEINLINE FGate::FGate(const bool bStartClosed) : bGateOpen(!bStartClosed) {}