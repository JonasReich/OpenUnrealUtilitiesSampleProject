// Copyright (c) 2021 Jonas Reich

#pragma once

#include "Algo/MaxElement.h"
#include "Algo/MinElement.h"
#include "Traits/IsInteger.h"

/**
 * Ring buffer that aggregates numeric data.
 * Useful e.g. for tracking data of the last X frames.
 */
template <typename ElementType, typename AllocatorType>
class TRingAggregator_Base
{
public:
	void Add(ElementType Element)
	{
		if (Storage.GetSlack())
		{
			Storage.AddUninitialized(1);
		}
		Storage[WriteIndex] = Element;
		WriteIndex = (WriteIndex + 1) % ArrayMax();
	}

	int32 Num() const
	{
		return Storage.Num();
	}

	bool HasData() const
	{
		return Num() > 0;
	}

	ElementType Get(int32 Index) const
	{
		return HasData() ? Storage[GetWrappedRingIndex(Index)] : 0;
	}

	ElementType Last() const
	{
		return Get(-1);
	}

	ElementType Oldest() const
	{
		if (Storage.GetSlack())
		{
			return Get(0);
		}
		return Get(WriteIndex);
	}
	
	ElementType Sum() const
	{
		ElementType ResultSum = 0;
		for (ElementType Element : Storage)
		{
			AddNumbersEnsured(ResultSum, Element);
		}
		return ResultSum;
	}
	
	ElementType Average() const
	{
		return HasData() ? (Sum() / Num()) : 0;
	}

	ElementType Max() const
	{
		return HasData() ? (*Algo::MaxElement(Storage)) : 0;
	}

	ElementType Min() const
	{
		return HasData() ? (*Algo::MinElement(Storage)) : 0;
	}

	const TArray<ElementType, AllocatorType>& GetStorage() const
	{
		return Storage;
	}

protected:
	TArray<ElementType, AllocatorType> Storage;
	int32 WriteIndex = 0;

	int32 ArrayMax() const
	{
		return Storage.GetSlack() + Num();
	}

	int32 GetWrappedRingIndex(int32 Index) const
	{
		const int32 RingIndex = (WriteIndex - 1 - Index);
		const int32 WrappedRingIndex = (RingIndex < 0) ? (RingIndex + ArrayMax()) : RingIndex;
		return WrappedRingIndex;
	}

	/** Overload for builds with ensure macros that checks for integer overflow */
	template<typename T, typename = typename TEnableIf<TIsInteger<T>::Value && DO_CHECK>::Type>
	static void AddNumbersEnsured(T& A, T B)
	{
		ElementType SignBefore = FMath::Sign(A);
		A += B;
		ElementType SignAfter = FMath::Sign(A);

		ensureMsgf(SignBefore != SignAfter && FMath::Abs(SignBefore) + FMath::Abs(SignAfter)
			&& SignBefore == FMath::Sign(B), TEXT("integer overflow detected"));
	}

	template<typename T>
    static void AddNumbersEnsured(T& A, T B, int32 = 0)
	{
		A += B;
	}
};

/** "Normal" ring aggregator that can be initialized with different size depending on dynamic conditions */
template<typename ElementType, int32 ElementNum>
class TRingAggregator : public TRingAggregator_Base<ElementType, FDefaultAllocator>
{
	using Super = TRingAggregator_Base<ElementType, FDefaultAllocator>;
	TRingAggregator(int32 InitialSize)
	{
		static_cast<Super*>(this)->Storage.SetNum(InitialSize);
	}
};

/** Ring aggregator that has a compile time fixed size of elements */
template<typename ElementType, int32 ElementNum>
class TFixedSizeRingAggregator : public TRingAggregator_Base<ElementType, TFixedAllocator<ElementNum>>
{
};
