#pragma once

#define USE_GOOGLE_DENSE_HASH_MAP
#define RESERVED_SIZE 32768

#ifdef USE_GOOGLE_DENSE_HASH_MAP
#include <sparsehash/dense_hash_map>
#include <sparsehash/dense_hash_set>
#define CUSTOM_SET google::dense_hash_set
#define CUSTOM_MAP google::dense_hash_map
#else
#include <map>
#include <set>
#define CUSTOM_SET std::set
#define CUSTOM_MAP std::map
#endif

#include <string>

#define getHashCode(item1Id, item2Id)\
	 std::hash<std::string>{}(item1Id < item2Id ? std::to_string(item1Id) + "/" + std::to_string(item2Id) : std::to_string(item2Id) + "/" + std::to_string(item1Id))

class SlopeOne {
public:
	SlopeOne()
	{
#ifdef USE_GOOGLE_DENSE_HASH_MAP
		difffMatrix.set_deleted_key(-1);
		difffMatrix.set_empty_key(0);

		items.set_deleted_key(-1);
		items.set_empty_key(0);
#endif

		difffMatrix.resize(RESERVED_SIZE);
		items.resize(RESERVED_SIZE);
	}

	void AddUsers(std::vector<CUSTOM_MAP<size_t, float>> users)
	{
		auto usersEnd = users.end();
		for (auto item = users.begin(); item != usersEnd; ++item)
			AddUserRatings(*item);
	}

	void AddUserRatings(CUSTOM_MAP<size_t, float> & userRatings)
	{
		auto userRatingsEnd = userRatings.end();
		for (auto item1 = userRatings.begin(); item1 != userRatingsEnd; ++item1)
		{
			size_t item1Id = item1->first;
			double item1Rating = item1->second;
			items.insert(item1->first);

			for (auto item2 = userRatings.begin(); item2 != userRatingsEnd; ++item2)
			{
				if (item2->first <= item1Id)
					continue;

				size_t item2Id = item2->first;
				float item2Rating = item2->second;

				std::shared_ptr<rating> ratingDiff;
				if (isKeyContain(item1Id, item2Id))
					ratingDiff = getValue(item1Id, item2Id);
				else
				{
					ratingDiff = std::make_shared<rating>();
					setValue(item1Id, item2Id, ratingDiff);
				}

				ratingDiff->Value += item1Rating - item2Rating;
				++ratingDiff->Freq;
			}
		}
	}

	CUSTOM_MAP<size_t, float> Predict(CUSTOM_MAP<size_t, float> userRatings)
	{
		CUSTOM_MAP<size_t, float> returnValue;
#ifdef USE_GOOGLE_DENSE_HASH_MAP
		returnValue.set_empty_key(-1);
		returnValue.set_deleted_key(0);
#endif
		auto difffMatrixEnd = difffMatrix.end();
		auto itemsEnd = items.end();
		auto userRatingsEnd = userRatings.end();
		for (auto itemId = items.begin(); itemId != itemsEnd; ++itemId)
		{
			if (userRatings.find(*itemId) != userRatingsEnd)
				continue;

			size_t item = *itemId;
			rating itemRating;

			for (auto userRating = userRatings.begin(); userRating != userRatingsEnd; ++userRating)
			{
				if (userRating->first == *itemId) 
					continue;

				int inputItemId = userRating->first;
				if (difffMatrix.find(getHashCode(*itemId, inputItemId)) != difffMatrixEnd)
				{
					std::shared_ptr<rating> diff = difffMatrix[getHashCode(*itemId, inputItemId)];
					itemRating.Value += diff->Freq * (userRating->second + diff->AverageValue() * ((*itemId < inputItemId) ? 1 : -1));
					itemRating.Freq += diff->Freq;
				}
			}
			
			returnValue.insert(std::pair<size_t, float>(*itemId, itemRating.AverageValue()));
		}

		return returnValue;
	}

	typedef class pRating
	{
	public:
		pRating() :Value(0), Freq(0) { }
		pRating(float value, size_t freq) : Value(value), Freq(freq) { }

		float Value;
		size_t Freq;

		float AverageValue() const
		{
			if (Value == 0.0 || Freq == 0)
				return 0.0;

			return Value / Freq;
		}

	} rating;

	bool isKeyContain(size_t item1Id, size_t item2Id)
	{
		return difffMatrix.find(getHashCode(item1Id, item2Id)) != difffMatrix.end();
	}

	std::shared_ptr<rating> getValue(size_t item1Id, size_t item2Id)
	{
		return difffMatrix[getHashCode(item1Id, item2Id)];
	}

	void setValue(size_t item1Id, size_t item2Id, const std::shared_ptr<rating> const & item)
	{
		difffMatrix.insert(std::pair<size_t, std::shared_ptr<rating>>(getHashCode(item1Id, item2Id), item));
	}

private:
	CUSTOM_MAP<size_t, std::shared_ptr<rating>> difffMatrix;
	CUSTOM_SET<size_t> items;
};
