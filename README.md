# SlopeOne

Usually Slope One algorithm using for collaborative filtering.
Only you need include header file to your project and add user rating datas via AddUserRatings function after than you can find similar products via Predict function.<br>
Large dataset takes too much time to prepare.

google::dense_hash_set using for performance improvements. If you don't want to use it only delete #define USE_GOOGLE_DENSE_HASH_MAP from SlopeOne.h<br>


##Usage
```c++
#include <iostream>
#include "SlopeOne.h"

using namespace std;


int main()
{
	SlopeOne slopeOne;

	CUSTOM_MAP<size_t, float> userRating;
#ifdef USE_GOOGLE_DENSE_HASH_MAP
	userRating.set_deleted_key(-1);
	userRating.set_empty_key(0);
#endif

	userRating.insert(pair<size_t, float>(1, 5.0));
	userRating.insert(pair<size_t, float>(2, 4.0));
	userRating.insert(pair<size_t, float>(3, 4.0));
	slopeOne.AddUserRatings(userRating);

	userRating = CUSTOM_MAP<size_t, float>();
#ifdef USE_GOOGLE_DENSE_HASH_MAP
	userRating.set_deleted_key(-1);
	userRating.set_empty_key(0);
#endif
	userRating.insert(pair<size_t, float>(1, 4.0));
	userRating.insert(pair<size_t, float>(2, 5.0));
	userRating.insert(pair<size_t, float>(3, 3.0));
	userRating.insert(pair<size_t, float>(4, 5.0));
	slopeOne.AddUserRatings(userRating);

	userRating = CUSTOM_MAP<size_t, float>();
#ifdef USE_GOOGLE_DENSE_HASH_MAP
	userRating.set_deleted_key(-1);
	userRating.set_empty_key(0);
#endif
	userRating.insert(pair<size_t, float>(1, 4.0));
	userRating.insert(pair<size_t, float>(2, 4.0));
	userRating.insert(pair<size_t, float>(4, 5.0));
	slopeOne.AddUserRatings(userRating);

	userRating = CUSTOM_MAP<size_t, float>();
#ifdef USE_GOOGLE_DENSE_HASH_MAP
	userRating.set_deleted_key(-1);
	userRating.set_empty_key(0);
#endif
	userRating.insert(pair<size_t, float>(1, 5.0));
	userRating.insert(pair<size_t, float>(3, 4.0));

	CUSTOM_MAP<size_t, float> predictions = slopeOne.Predict(userRating);
	for (auto it = predictions.begin(); it != predictions.end(); ++it)
		cout << "Item " << it->first << " Rating: " << it->second << endl;

    return 0;
}
```


##License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License, 
version 3.0 (LGPL-3.0) as published by the Free Software Foundation.
[http://www.opensource.org/licenses/LGPL-3.0](http://www.opensource.org/licenses/LGPL-3.0)
