#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionData.h"

// Player class, combine Body, Keyboard, and Player State
class Player {
private:
	// The world this player belong to
	b2World* const m_world_;

	//Player color
	b2Color m_color_;

	//b2Body* body;
	b2Body* body;

	const b2Vec2 default_lv{15.1f, 0.0f};

	// Keyboard keys in order : Move Forward, Move Backward, Turn Left, Turn Right
	char binding_key[5];

	//Initial keyboard state
	char keyboard_state_ = 0;

public:
	/// Create a body as a Player and bind keyboard keys
	/// @param world in which world
	/// @param keyboard Keyboard keys to control Player. WARNING: This char array MUST have EXACTLY four elements and one '\0'
	/// @param position this player will be set to where
	/// @param color the color of this player
	Player(b2World* const world, const char keyboard[3], const b2Vec2& position, const b2Color& color = { 0.9f, 0.7f, 0.7f }) :
		m_world_{ world }, m_color_{ color } {
		// Bind keyboard
		memcpy(binding_key, keyboard, sizeof(char) * 3);

		// Create body
		b2BodyDef bodydef;
		bodydef.position = position;
		bodydef.type = b2_dynamicBody;
		bodydef.angularDamping = 15.0f;
		bodydef.linearDamping = 0.3f;
		body = world->CreateBody(&bodydef);
		{
			b2PolygonShape shape;
			shape.SetAsBox(2.0f, 0.2f, b2Vec2(0.8f, 0.3f), 0);
			b2FixtureDef fixturedef;
			fixturedef.shape = &shape;
			fixturedef.restitution = 1.1;
			fixturedef.density = 3.0f;
			fixturedef.friction = 0.f;
			body->CreateFixture(&fixturedef);
		}

		// Water Ceiling
		{
			b2BodyDef bd;
			bd.position.Set(0, 5);
			b2Body* ground = m_world_->CreateBody(&bd);
			ground->SetUserData(new CollisionData{ ~0u });

			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(-10, -9.5),
					b2Vec2(-8, -9.5),
					b2Vec2(-8, -9),
					b2Vec2(-10, -9) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(-5, -9.5),
					b2Vec2(-3, -9.5),
					b2Vec2(-3, -9),
					b2Vec2(-5, -9) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(-1, -9.5),
					b2Vec2(1, -9.5),
					b2Vec2(1, -9),
					b2Vec2(-1, -9) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(3, -9.5),
					b2Vec2(5, -9.5),
					b2Vec2(5, -9),
					b2Vec2(3, -9) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(8, -9.5),
					b2Vec2(10, -9.5),
					b2Vec2(10, -9),
					b2Vec2(8, -9) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}

			// Restrict paddle along the x axis
			b2PrismaticJointDef jointDef;
			b2Vec2 worldAxis(1.0f, 0.0f);
			jointDef.collideConnected = true;
			jointDef.Initialize(body, ground, 
			body->GetWorldCenter(), worldAxis);
			m_world_->CreateJoint(&jointDef);
		}

		SetColor(m_color_);
	}

	Player(const Player& other) = delete;
	Player(Player&& other) = delete;

	// Destruct Player's Body and ParticleSystem
	~Player() {
		m_world_->DestroyBody(this->body);
		this->body = nullptr;
	}

	void Move(const b2Vec2& lv) {
		body->SetLinearVelocity(lv);
	}
	// Move to left by default linear velocity (default_lv)
	void LeftMove() {
		Move(-default_lv);
	}
	// Move to right by default linear velocity (default_lv)
	void RightMove() {
		Move(default_lv);
	}

	// Operate according to current keyboard state
	void KeyBoardOperator() {
		if (keyboard_state_ & (1 << 0)) {
			LeftMove();
		}
		if (keyboard_state_ & (1 << 1)) {
			RightMove();
		}
	}

	// Update keyboard state with a key press
	void KeyBoardPress(char key) {
		for (uint32 i = 0; i < 2; ++i) {
			if (key == binding_key[i]) {
				keyboard_state_ |= (1 << i);
			}
		}
	}

	// Update keyboard state with a key release
	void KeyBoardRelease(char key) {
		for (uint32 i = 0; i < 2; ++i) {
			if (key == binding_key[i]) {
				keyboard_state_ &= (~0) ^ (1 << i);
			}
		}
	}

	void Maintain(const float32& dt) {
		KeyBoardOperator();
	}

	// Print information to screen
	void PrintInformation(DebugDraw* debug_draw, int& text_line) {
		char buffer[256];
		//Print Player Group
		sprintf(buffer, "Player :  ");
		debug_draw->DrawString(5, text_line, buffer);
		text_line += DRAW_STRING_NEW_LINE;
	}

	// Set the color of this player
	void SetColor(const b2Color& color) {
		m_color_ = color;
	}
};

#endif