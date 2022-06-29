#ifndef OBSTACLE_H
#define OBSTACLE_H

// A class used to manage obstacles in the world
class Obstacle {
private:
	b2World* m_world_;
	b2Body* m_body_;
public:
	Obstacle(b2World* const world, const float32& x, const float32& y) :
		m_world_{ world } {
		b2BodyDef bd;
		bd.position.Set(x, y);
		bd.type = b2_kinematicBody;
		bd.angularDamping = 0.0f;
		bd.linearDamping = 0.2f;
		m_body_ = world->CreateBody(&bd);
		m_body_->SetUserData(new CollisionData{ 0u });

		{
			b2PolygonShape shape;
			shape.SetAsBox(1.0f, 0.5f);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.restitution = 1;
			fd.density = 3.0f;
			fd.friction = 0.5f;
			m_body_->CreateFixture(&fd);
		}

	}

	Obstacle(const Obstacle& other) = delete;
	Obstacle(Obstacle&& other) = delete;

	~Obstacle() {
		m_world_->DestroyBody(m_body_);
		m_body_ = nullptr;
		m_world_ = nullptr;
	}

	// Get whether obstacle this need to be deleted
	bool IsNeedDelete() {
		return static_cast<CollisionData*>(m_body_->GetUserData())->IsNeedDelete();
	}
};

#endif