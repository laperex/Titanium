#pragma once

#include "System.h"

namespace ti {
	namespace System {
		class Physics {
			ti::ECS::Registry* registry;

		public:
			void Init(ti::ECS::Registry* registry) {
				this->registry = registry;
			}

			void Update(double dt);
			void GetCollisionEntities();
		};
	}
}