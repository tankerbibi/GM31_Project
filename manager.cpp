#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "field.h"

Polygon2D* g_Polygon2D;
Field* g_Field;

void Manager::Init()
{
	Renderer::Init();
	g_Polygon2D = new Polygon2D();
	g_Polygon2D->Init();
	g_Field = new Field();
	g_Field->Init();
}


void Manager::Uninit()
{
	g_Polygon2D->Uninit();
	delete g_Polygon2D;
	g_Field->Uninit();
	delete g_Field;
	Renderer::Uninit();
}

void Manager::Update()
{
	g_Polygon2D->Update();
	g_Field->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	g_Polygon2D->Draw();
	g_Field->Draw();
	Renderer::End();
}
