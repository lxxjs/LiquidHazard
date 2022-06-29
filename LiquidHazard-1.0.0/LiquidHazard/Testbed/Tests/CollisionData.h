#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

// A class to record the number of collision times with flag in consider
class CollisionData {
private:
	// Player Group
	uint32 flag_;
	// Delete tag
	bool need_delete_ = 0;
public:
	explicit CollisionData(const uint32 flag = 0) : flag_{ flag } {}

	uint32 GetFlag() const {
		return flag_;
	}

	void SetFlag(uint32 flag) {
		flag_ = flag;
	}

	// Get whether this need to be deleted
	bool IsNeedDelete() {
		return need_delete_;
	}

	// Mark as need to be deleted
	void Delete() {
		need_delete_ = 1;
	}
};

#endif