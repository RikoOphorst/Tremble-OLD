#include "pch.h"

#include "components\player.h"
#include "components\flying.h"
#include "components\rotate.h"
#include "components\output_position_and_rotation.h"

using namespace engine; 

class PointLightController : public Component
{
public:
	void Update()
	{
		Light* light = GetNode()->FindComponent<Light>();

		if (light != nullptr)
			light->SetPosition(Vector4(sinf(static_cast<float>(Get::TimeSinceStartup())) * 60.0f, 3.0f, 0.0f));
	}
};

SGNode* AddPhysicsBox(Vector3 position, Vector3 size = Vector3(1, 1, 1), Vector3 rotation = Vector3(), bool is_static = false)
{
	SGNode* box = Get::Scene()->AddChild(is_static, position, rotation, size);
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box, is_static);
	if (is_static)
	{
		box->AddComponent<RigidbodyStatic>(size / 2);
	}
	else
	{
		box->AddComponent<RigidbodyDynamic>();
	}
	return box;
}

void AddTarget(Vector3 position)
{
	AddPhysicsBox(Vector3(position.GetX(), position.GetY() / Scalar(2.0f), position.GetZ()), Vector3(0.1f, position.GetY().Get(), 0.1f), Vector3(), true);
	position.SetY(position.GetY().Get() + 0.2f);
	AddPhysicsBox(position, Vector3(0.2f, 0.4f, 0.2f));
}

void CheckRotations()
{
	SGNode* box1 = Get::Scene()->AddChild();
	box1->RotateXDegrees(Scalar(45));
	box1->RotateYDegrees(Scalar(45));
	box1->RotateZDegrees(Scalar(45));
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box1, false);
	box1->SetPosition(Vector3(-2, 0, 0));
	box1->AddComponent<Rotate>(1, 0, 0);

	SGNode* box2 = Get::Scene()->AddChild();
	box2->RotateXDegrees(Scalar(45));
	box2->RotateYDegrees(Scalar(45));
	box2->RotateZDegrees(Scalar(45));
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box2, false);
	box2->SetPosition(Vector3(0, 0, 0));
	box2->AddComponent<Rotate>(0, 1, 0);

	SGNode* box3 = Get::Scene()->AddChild();
	box3->RotateXDegrees(Scalar(45));
	box3->RotateYDegrees(Scalar(45));
	box3->RotateZDegrees(Scalar(45));
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box3, false);
	box3->SetPosition(Vector3(2, 0, 0));
	box3->AddComponent<Rotate>(0, 0, 1);

	SGNode* box1_2 = Get::Scene()->AddChild();
	box1_2->RotateXDegrees(Scalar(45));
	box1_2->RotateYDegrees(Scalar(45));
	box1_2->RotateZDegrees(Scalar(45));
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box1_2, false);
	box1_2->SetPosition(Vector3(-2, 0, 2));
	box1_2->AddComponent<Rotate2>(1, 0, 0);

	SGNode* box2_2 = Get::Scene()->AddChild();
	box2_2->RotateXDegrees(Scalar(45));
	box2_2->RotateYDegrees(Scalar(45));
	box2_2->RotateZDegrees(Scalar(45));
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box2_2, false);
	box2_2->SetPosition(Vector3(0, 0, 2));
	box2_2->AddComponent<Rotate2>(0, 1, 0);

	SGNode* box3_2 = Get::Scene()->AddChild();
	box3_2->RotateXDegrees(Scalar(45));
	box3_2->RotateYDegrees(Scalar(45));
	box3_2->RotateZDegrees(Scalar(45));
	FBXLoader::LoadFBXScene("../../models/cube_textured/cube_textured.fbx", box3_2, false);
	box3_2->SetPosition(Vector3(2, 0, 2));
	box3_2->AddComponent<Rotate2>(0, 0, 1);
}

int main(int argc, char* argv) 
{
	GameManager* game_manager = GameManager::Create(1000000000);

	game_manager->Startup("Duty Calls: The FPS", 1280, 720);
	game_manager->LoadScene(Get::ConfigManager()->GetScenes()[Get::Config().scene]);

	if (Get::Config().mouse_lock)
	{
		//Get::InputManager()->HideCursor();
		Get::InputManager()->LockCursor();
	}

	SGNode* child = Get::Scene()->AddChild();
	child->AddComponent<StaticPlane>();

	SGNode* player = Get::Scene()->AddChild();

	player->AddComponent<RaycastShootDynamics>();

	SGNode* player_camera = player->AddChild(false, Vector3(0.0f, 0.4f, 0.0f));
	Camera* camera = player_camera->AddComponent<Camera>();
	player->SetPosition(Vector3(0, 1, 0));

	if (!Get::Config().free_camera)
	{
		player->AddComponent<CharacterController>();
		player->AddComponent<Player>();
	}
	else
	{
		Flying* flying = player->AddComponent<Flying>();
	}

	camera->SetFOV(60.0f * 0.0174533f);


	AddTarget(Vector3(2.0f, 2.0f, 4.0f));
	AddTarget(Vector3(1.0f, 2.0f, 4.0f));
	AddTarget(Vector3(0.0f, 2.0f, 4.0f));

	AddTarget(Vector3(2.0f, 4.0f, 8.0f));
	AddTarget(Vector3(1.0f, 4.0f, 8.0f));
	AddTarget(Vector3(0.0f, 4.0f, 8.0f));

	AddTarget(Vector3(8.0f, 4.0f, 1.0f));
	AddTarget(Vector3(8.0f, 5.0f, 2.0f));
	AddTarget(Vector3(8.0f, 6.0f, 3.0f));

	AddTarget(Vector3(15.0f, 4.0f, 1.0f));
	AddTarget(Vector3(15.0f, 5.0f, 2.0f));
	AddTarget(Vector3(15.0f, 6.0f, 3.0f));

	AddTarget(Vector3(-8.0f, 4.0f, 1.0f));
	AddTarget(Vector3(-8.0f, 5.0f, 2.0f));
	AddTarget(Vector3(-8.0f, 6.0f, 3.0f));

	Get::Renderer()->SetCamera(camera);

	SGNode* n = Get::Scene()->AddChild();
	Light* l = n->AddComponent<Light>();
	PointLightController* plc = n->AddComponent<PointLightController>();
		
	l->SetLightType(LightTypePoint);
	l->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	l->SetPosition(Vector4(0.0f, 50.0f, 0.0f, 1.0f));
	l->SetFalloffStart(Scalar(0.0f));
	l->SetFalloffEnd(Scalar(250.0f));
	
	game_manager->MainLoop();

	game_manager->ShutDown();

	GameManager::Delete(game_manager);

	return 0;
}