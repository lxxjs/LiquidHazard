#ifndef LIQUID_HAZARD_H
#define LIQUID_HAZARD_H

#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "CollisionData.h"
#include "Player.h"
#include "Obstacle.h"

int obstacle_count = 0;
bool alive = true;

class MyContactListener : public b2ContactListener {
private:
	std::vector<b2Body*> m_toDelete;

public:
	// When the ball hits the water or the elastic red ball, end the game
	void BeginContact(b2ParticleSystem * particleSystem, b2ParticleBodyContact * particleBodyContact )
	{
		auto b2Body = particleBodyContact->body->GetUserData();
		if (b2Body){
			if (static_cast<CollisionData*>(b2Body)->GetFlag() == 7) {
					alive = false;
			}
		}
	}
	
	// When the ball hits the obtacle
	void EndContact(b2Contact* contact)
	{
		auto b2BodyA = contact->GetFixtureA()->GetBody()->GetUserData();
		auto b2BodyB = contact->GetFixtureB()->GetBody()->GetUserData();

		if (b2BodyA) { 
			if (static_cast<CollisionData*>(b2BodyA)->GetFlag() == 0) {
				CollisionData* UserDataA = static_cast<CollisionData*>(b2BodyA);
				if (UserDataA){
					obstacle_count++;
					m_toDelete.push_back(contact->GetFixtureA()->GetBody());
				}
			}
		} else if (b2BodyB) { 
			if (static_cast<CollisionData*>(b2BodyB)->GetFlag() == 0) {
				CollisionData* UserDataB = static_cast<CollisionData*>(b2BodyB);
				if (UserDataB){
					obstacle_count++;
					m_toDelete.push_back(contact->GetFixtureB()->GetBody());
				}
			}
		}
		DeleteCollidedBodies();
	}
	void DeleteCollidedBodies()
	{
		for(auto& body : m_toDelete)
		{	
			CollisionData* UserData = static_cast<CollisionData*>(body->GetUserData());
			UserData->Delete();
		}
		m_toDelete.clear();
	}
};

class LiquidHazard : public Test {
protected:
	b2Body* m_ground;
private:
	std::vector<Player*> player_list_;
	std::list<Obstacle*> obstacle_list_;

	int32 obstacle_generation_interval_ = 0x1ff;

	// Calculate a random number in [l, r].
	float32 Random(const float32& l, const float32& r) {
		float32 x = ((float32)rand() / (float32)RAND_MAX);
		return l + (r - l) * x;
	}

public:
	MyContactListener m_contact_listener_;
	b2FixtureDef fixturedef;
	LiquidHazard() {
		obstacle_count = 0;
		alive = true;
		m_world->SetContactListener( &m_contact_listener_ );
		player_list_.push_back(new Player{ m_world, "ad", b2Vec2{0.0f, -4.0f} });

		// Outer Walls
		{
			b2BodyDef bd;
			bd.position.Set(0, 5);
			b2Body* ground = m_world->CreateBody(&bd);
			ground->SetUserData(new CollisionData{ ~0u });

			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(-11, -16),
					b2Vec2(11, -16),
					b2Vec2(11, -15),
					b2Vec2(-11, -15) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(-11, 15),
					b2Vec2(11, 15),
					b2Vec2(11, 16),
					b2Vec2(-11, 16) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(-11, -15),
					b2Vec2(-10, -15),
					b2Vec2(-10, 15),
					b2Vec2(-11, 15) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
			{
				b2PolygonShape shape;
				const b2Vec2 vertices[4] = {
					b2Vec2(11, -15),
					b2Vec2(10, -15),
					b2Vec2(10, 15),
					b2Vec2(11, 15) };
				shape.Set(vertices, 4);
				ground->CreateFixture(&shape, 0.0f);
			}
		}

		// Water
		{
			m_particleSystem->SetRadius(0.05f);
			m_particleSystem->SetDensity(1.0f);
			{
				b2PolygonShape shape;
				shape.SetAsBox(10, 1.5);
				b2ParticleGroupDef pd;
				pd.flags = b2_waterParticle | b2_fixtureContactListenerParticle | b2_particleContactFilterParticle;
				pd.position.Set(0, -7);
				pd.shape = &shape;
				pd.color.Set(0, 0, 255, 255);
				pd.userData = new CollisionData(3u);
				m_particleSystem->CreateParticleGroup(pd);
			}
		}
		
		// Elastic ball
		{
			{
				b2CircleShape shape;
				shape.m_p.Set(0, 10);
				shape.m_radius = 0.5f;
				b2ParticleGroupDef pd;
				pd.flags = b2_elasticParticle | b2_particleContactFilterParticle;
				pd.groupFlags = b2_solidParticleGroup;
				pd.shape = &shape;
				pd.color.Set(255, 0, 0, 255);
				pd.strength = 1.0f;
				pd.userData = new CollisionData( 2u );
				m_particleSystem->CreateParticleGroup(pd);
			}
		}

		// Ball
		{
			b2BodyDef bd;
			b2CircleShape shape;
			shape.m_radius = 0.5f;
			bd.position.Set(0, 0);
			bd.type = b2_dynamicBody;
			b2Body* body = m_world->CreateBody(&bd);
			body->SetUserData(new CollisionData{ 7u });
			b2FixtureDef fixturedef;
			fixturedef.shape = &shape;
			fixturedef.restitution = 1;
			fixturedef.density = 0.5f;
			fixturedef.friction = 0.f;
			body->CreateFixture(&fixturedef);
			b2Vec2 force = b2Vec2(3, 10);
			body->ApplyLinearImpulse(force, bd.position, 1);
		}
	}
	
	// Change the keyboard state
	void Keyboard(unsigned char key) {
		for (auto x : player_list_) {
			x->KeyBoardPress(key);
		}
	}
	// Change the keyboard state
	void KeyboardUp(unsigned char key) {
		for (auto x : player_list_) {
			x->KeyBoardRelease(key);
		}
	}

	// Print current score to screen
	void PrintInformation() {
		char buffer[256];
		sprintf(buffer, "Points earned: %d ", obstacle_count);
		m_debugDraw.DrawString(5, m_textLine, buffer);
	}

	void Step(Settings* settings) {
		Test::Step(settings);
		const float32 dt = 1.0f / settings->hz;
		for (auto x : player_list_) {
			x->Maintain(dt);
		}
		for (auto x = obstacle_list_.begin(); x != obstacle_list_.end();) {
			if ((*x)->IsNeedDelete()) {
				delete (*x);
				x = obstacle_list_.erase(x);
			}
			else {
				++x;
			}
		}
		PrintInformation();
		if ((m_stepCount & obstacle_generation_interval_) == obstacle_generation_interval_) {
			float32 numbers[10] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};
			for (int row=0; row<=4; row++){
				srand((unsigned)time(0));
				int max_index = 9;
				int rand_length = rand() % 10 + 1;
				float32 * random_numbers = new float32[rand_length];
				int count = 0;
				while (count < rand_length){
					int j = rand() % (max_index+1);
					random_numbers[count] = numbers[j];
					float32 temp = numbers[j];
					numbers[j] = numbers[max_index];
					numbers[max_index] = temp;
					max_index--;
					count++;
				}
				std::cout << "**Generate Obstacles**" << std::endl;
				for (int i=0; i < rand_length; i++){
					obstacle_list_.push_back(new Obstacle{m_world, -8.5f + 1.92f *(random_numbers[i]-1), 14.5f-1.2f*row});
				}
			}
		
		}
		if (!alive) {
			settings->pause = 1;
			//Print Player Die
			char buffer[256];
			sprintf(buffer, "Game Over !");
			m_textLine += DRAW_STRING_NEW_LINE;
			m_debugDraw.DrawString(5, m_textLine, buffer);
			m_textLine += DRAW_STRING_NEW_LINE;
		}
	}

	float32 GetDefaultViewZoom() const {
		return 0.7f;
	}

	static Test* Create() {
		return new LiquidHazard;
	}
};

#endif