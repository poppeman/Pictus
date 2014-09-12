#include "StdAfx.h"
#include "main.h"
#include "Pictus/w32_rs_tilemanager.h"
#include "Pictus/w32_renderer.h"
#include "Pictus/w32_ddsurface.h"

SUITE(TileManager) {
	using Win::TileManager;
	using Win::DDSurface;
	using namespace Geom;
	using Win::Renderer;

	class StubDDSurface:public DDSurface {
	private:
		void OnCreate(const Geom::SizeInt& ) {}
	};

	class StubRenderer:public Renderer {
	public:
		typedef std::shared_ptr<StubRenderer> Ptr;

		struct PresentCall {
			Geom::RectInt destRect;
			DDSurface::Ptr source;
			Geom::PointInt sourceTL;
		};
		typedef std::deque<PresentCall> PresentList;

		PresentList pcalls;

		Img::Surface::Ptr CreateSurface() {
			return Img::Surface::Ptr();
		}
		DDSurface::Ptr OnCreateDDSurface() {
			return DDSurface::Ptr(new StubDDSurface);
		}

		Geom::SizeInt RenderAreaSize() {
			throw std::runtime_error("Don't call this.");
		}
		void EnablePan() {
			m_pan = true;
		}
		bool PanCurrentView(const Geom::SizeInt& ) { return m_pan; }

		void OnRenderToDDSurface(DDSurface::Ptr , Img::Surface::Ptr , const Geom::PointInt& , const Geom::RectInt& , const Img::Properties& ) {
		}
		void OnPresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft) {
			PresentCall p = {destRect, source, sourceTopLeft};
			pcalls.push_back(p);
		}

		RenderStatus OnBeginRender(Img::Color) override { return Renderer::RS_OK; }
		void OnEndRender() {}

		StubRenderer():m_pan(false) {}
	private:
		bool m_pan;
	};

	struct SimpleFixture {
		SimpleFixture() {
			r.reset(new StubRenderer);
			m.reset(new TileManager(r));
		}
		TileManager::Ptr m;
		StubRenderer::Ptr r;
	};

	struct SimplePanFixture {
		SimplePanFixture() {
			r.reset(new StubRenderer);
			m.reset(new TileManager(r));
			m->SetViewportSize(SizeInt(400, 400));
			r->EnablePan();
		}
		TileManager::Ptr m;
		StubRenderer::Ptr r;
	};


	TEST_FIXTURE(SimpleFixture, InvalidViewPort) {
		CHECK_THROW(m->SetViewportSize(SizeInt(0, 0)), Err::InvalidParam);
	}
	TEST(InvalidRenderer) {
		CHECK_THROW(TileManager::Ptr m(new TileManager(Renderer::Ptr())), Err::InvalidParam);
	}

	TEST_FIXTURE(SimpleFixture, TileSizeClip) {
		m->SetViewportSize(SizeInt(1000, 1000));
		TileManager::RequestedArea ar = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(800, 800)));
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(512, 512)), ar.WriteableArea);
	}

	TEST_FIXTURE(SimpleFixture, TileViewportClip) {
		m->SetViewportSize(SizeInt(400, 400));
		TileManager::RequestedArea ar = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(500, 500)));
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(400, 400)), ar.WriteableArea);
	}


	TEST_FIXTURE(SimpleFixture, MultipleRequests) {
		m->SetViewportSize(SizeInt(400, 400));
		TileManager::RequestedArea ar = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(256, 256)));
		ar = m->RequestDDSurface(RectInt(PointInt(256, 256), SizeInt(512, 512)));
		m->Render({ 0, 0 });
		CHECK_EQUAL(1, r->pcalls.size());
		if(r->pcalls.size() < 1) return;
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(400, 400)), r->pcalls[0].destRect);
		CHECK_EQUAL(PointInt(0, 0), r->pcalls[0].sourceTL);
	}



	TEST_FIXTURE(SimpleFixture, TileSplit) {
		m->SetViewportSize(SizeInt(600, 100));
		TileManager::RequestedArea a = m->RequestDDSurface(RectInt(PointInt(511, 0), SizeInt(50, 200)));
		CHECK_EQUAL(RectInt(PointInt(511, 0), SizeInt(1, 100)), a.WriteableArea);

		m->AddOffset(SizeInt(-1, 0));
		a = m->RequestDDSurface(RectInt(PointInt(511, 0), SizeInt(50, 200)));
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(50, 100)), a.WriteableArea);
	}

	TEST_FIXTURE(SimpleFixture, PannedTileSplit) {
		m->SetViewportSize(SizeInt(400, 400));
		m->AddOffset(SizeInt(-10, 0));
		TileManager::RequestedArea a = m->RequestDDSurface(RectInt(PointInt(390, 0), SizeInt(10, 400)));
		CHECK_EQUAL(RectInt(PointInt(400, 0), SizeInt(10, 400)), a.WriteableArea);
	}

	TEST_FIXTURE(SimpleFixture, TestFull) {
		m->SetViewportSize(SizeInt(470, 93));
		TileManager::RequestedArea a = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(470, 93)));
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(470, 93)), a.WriteableArea);
	}

	TEST_FIXTURE(SimpleFixture, TestWrappingOffset) {
		m->SetViewportSize(SizeInt(300, 308));
		m->AddOffset(SizeInt(508, 510));
		TileManager::RequestedArea a = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(300, 2)));
		CHECK_EQUAL(RectInt(PointInt(4, 2), SizeInt(300, 2)), a.WriteableArea);
	}

	TEST_FIXTURE(SimpleFixture, TestSinglePixel) {
		m->SetViewportSize(SizeInt(600, 600));
		DDSurface::Ptr s1 = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(1, 1))).Surface;
		DDSurface::Ptr s2 = m->RequestDDSurface(RectInt(PointInt(520, 0), SizeInt(1, 1))).Surface;
		DDSurface::Ptr s3 = m->RequestDDSurface(RectInt(PointInt(0, 520), SizeInt(1, 1))).Surface;
		DDSurface::Ptr s4 = m->RequestDDSurface(RectInt(PointInt(520, 520), SizeInt(1, 1))).Surface;

		CHECK(s1 != s2);
		CHECK(s1 != s3);
		CHECK(s1 != s4);
		CHECK(s2 != s3);
		CHECK(s2 != s4);
		CHECK(s3 != s4);

		for(int y = 500; y < 600; y++) {
			for(int x = 500; x < 600; x++) {
				TileManager::RequestedArea a = m->RequestDDSurface(RectInt(PointInt(x, y), SizeInt(1, 1)));
				CHECK_EQUAL(SizeInt(1, 1), a.WriteableArea.Dimensions());
				
				CHECK_EQUAL(PointInt(x<512?x:x-512, y<512?y:y-512), a.WriteableArea.TopLeft());
				if(x < 512 && y < 512)
					CHECK_EQUAL(s1, a.Surface);
				else if(y < 512)
					CHECK_EQUAL(s2, a.Surface);
				else if(x < 512)
					CHECK_EQUAL(s3, a.Surface);
				else
					CHECK_EQUAL(s4, a.Surface);
			}
		}
	}

	TEST_FIXTURE(SimpleFixture, TestSimpleRender) {
		m->SetViewportSize(SizeInt(400, 400));
		m->AddOffset(SizeInt(10, 20));
		m->Render({ 0, 0 });
		CHECK_EQUAL(4, r->pcalls.size());

		if(r->pcalls.size() != 4) return;
		CHECK_EQUAL(m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(1, 1))).Surface, r->pcalls[0].source);
		CHECK_EQUAL(PointInt(0, 0), r->pcalls[0].destRect.TopLeft());
		CHECK_EQUAL(PointInt(10, 0), r->pcalls[1].destRect.TopLeft());
		CHECK_EQUAL(PointInt(0, 20), r->pcalls[2].destRect.TopLeft());
		CHECK_EQUAL(PointInt(10, 20), r->pcalls[3].destRect.TopLeft());
	}

	TEST_FIXTURE(SimplePanFixture, TestNOOPRender) {
		m->Render({ 0, 0 });
		CHECK_EQUAL(0, r->pcalls.size());
	}


	TEST_FIXTURE(SimplePanFixture, TestPanNOOPRender) {
		m->AddOffset(SizeInt(10, 10));
		m->Render({ 0, 0 });
		CHECK_EQUAL(0, r->pcalls.size());
	}

	TEST_FIXTURE(SimplePanFixture, TestDefaultRender) {
		DDSurface::Ptr s = m->RequestDDSurface(RectInt(PointInt(10, 20), SizeInt(30, 40))).Surface;
		m->Render({ 0, 0 });
		CHECK_EQUAL(1, r->pcalls.size());
		if(r->pcalls.size() < 1) return;
		CHECK_EQUAL(s, r->pcalls[0].source);
		CHECK_EQUAL(RectInt(PointInt(10, 20), SizeInt(30, 40)), r->pcalls[0].destRect);
		CHECK_EQUAL(PointInt(10, 20), r->pcalls[0].sourceTL);
	}

	TEST_FIXTURE(SimplePanFixture, TestHorizontalPanRenderNegative) {
		m->AddOffset(SizeInt(-10, 0));
		DDSurface::Ptr s = m->RequestDDSurface(RectInt(PointInt(390, 0), SizeInt(10, 400))).Surface;
		m->Render({ 0, 0 });
		CHECK_EQUAL(1, r->pcalls.size());

		if(r->pcalls.size() != 1) return;
		CHECK_EQUAL(s, r->pcalls[0].source);
		CHECK_EQUAL(RectInt(PointInt(390, 0), SizeInt(10, 400)), r->pcalls[0].destRect);
		CHECK_EQUAL(PointInt(400, 0), r->pcalls[0].sourceTL);
	}

	TEST_FIXTURE(SimplePanFixture, TestHorizontalPanRenderPositive) {
		m->AddOffset(SizeInt(10, 0));
		DDSurface::Ptr s = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(10, 400))).Surface;
		m->Render({ 0, 0 });
		CHECK_EQUAL(1, r->pcalls.size());

		if(r->pcalls.size() != 1) return;
		CHECK_EQUAL(s, r->pcalls[0].source);
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(10, 400)), r->pcalls[0].destRect);
		CHECK_EQUAL(PointInt(502, 0), r->pcalls[0].sourceTL);
	}

	TEST_FIXTURE(SimplePanFixture, TestVerticalPanRenderNegative) {
		m->AddOffset(SizeInt(0, -10));
		DDSurface::Ptr s = m->RequestDDSurface(RectInt(PointInt(0, 390), SizeInt(400, 10))).Surface;
		m->Render({ 0, 0 });
		CHECK_EQUAL(1, r->pcalls.size());

		if(r->pcalls.size() != 1) return;
		CHECK_EQUAL(s, r->pcalls[0].source);
		CHECK_EQUAL(RectInt(PointInt(0, 390), SizeInt(400, 10)), r->pcalls[0].destRect);
		CHECK_EQUAL(PointInt(0, 400), r->pcalls[0].sourceTL);
	}

	TEST_FIXTURE(SimplePanFixture, TestVerticalPanRenderPositive) {
		m->AddOffset(SizeInt(0, 10));
		DDSurface::Ptr s = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(400, 10))).Surface;
		m->Render({ 0, 0 });
		CHECK_EQUAL(1, r->pcalls.size());

		if(r->pcalls.size() != 1) return;
		CHECK_EQUAL(s, r->pcalls[0].source);
		CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(400, 10)), r->pcalls[0].destRect);
		CHECK_EQUAL(PointInt(0, 502), r->pcalls[0].sourceTL);
	}


	TEST_FIXTURE(SimplePanFixture, TestDiagonalPanRenderPositive) {
		m->AddOffset(SizeInt(10, 10));
		struct surf {
			DDSurface::Ptr s;
			bool verified;
		};
		surf surfs[3];
		surfs[0].s = m->RequestDDSurface(RectInt(PointInt(0, 0), SizeInt(400, 400))).Surface;
		surfs[1].s = m->RequestDDSurface(RectInt(PointInt(0, 10), SizeInt(10, 400))).Surface;
		surfs[2].s = m->RequestDDSurface(RectInt(PointInt(10, 0), SizeInt(400, 10))).Surface;
		for(int i=0;i<3;i++) surfs[i].verified = false;

		m->Render({ 0, 0 });
		CHECK_EQUAL(3, r->pcalls.size());
		if(r->pcalls.size() != 3) return;
		for(int i=0;i<3;i++) {
			if(surfs[0].s == r->pcalls[i].source) {
				CHECK_EQUAL(surfs[0].s, r->pcalls[i].source);
				CHECK_EQUAL(RectInt(PointInt(0, 0), SizeInt(10, 10)), r->pcalls[i].destRect);
				CHECK_EQUAL(PointInt(502, 502), r->pcalls[i].sourceTL);
				surfs[0].verified = true;
			}
			else if(surfs[1].s == r->pcalls[i].source) {
				CHECK_EQUAL(surfs[1].s, r->pcalls[i].source);
				CHECK_EQUAL(RectInt(PointInt(0, 10), SizeInt(10, 390)), r->pcalls[i].destRect);
				CHECK_EQUAL(PointInt(502, 0), r->pcalls[i].sourceTL);
				surfs[1].verified = true;
			}
			else if(surfs[2].s == r->pcalls[i].source) {
				CHECK_EQUAL(surfs[2].s, r->pcalls[i].source);
				CHECK_EQUAL(RectInt(PointInt(10, 0), SizeInt(390, 10)), r->pcalls[i].destRect);
				CHECK_EQUAL(PointInt(0, 502), r->pcalls[i].sourceTL);
				surfs[2].verified = true;
			}
		}


	}
}
