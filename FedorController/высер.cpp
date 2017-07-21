// --- Воспроизведение ---
	while (!drivemag.eof())
	{

		drivemag >> motor >> time >> pos;
		getline(drivemag, wtf);


		time *= 5000;

		if (drivemag.eof())
		{
			AddDrive(poses[curPose], motor, pos);
			lastTime = time;
		}

		// Считаны весь фрейм - меняем буферы
		if (time > lastTime | drivemag.eof())
		{
			if (!isInterpolation)
			{
				//Без интерполяции
				int sleepTime = (time - lastTime) * 1000;
				callback(time, sleepTime , poses[curPose]);
				Sleep(sleepTime);

			}
			else if (isBoth)
			{
				// Интерполяция и отправка

				auto & fromPose = poses[abs(curPose - 1)];		// Из какой позиции 
				auto & toPose = poses[curPose];					// В какую переходить

				// Интерполяция
				int sleepTime = (lastTime - lastFrameTime) * 1000;
				auto t0 = chrono::steady_clock::now();
				double t;

				int i = 0;

				do
				{
					t = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - t0).count();

					for (auto & m : fromPose)
					{
						double from = m.second;
						double to = toPose[m.first];
						double interpolated = (to - from) * t / (double)sleepTime + from;

						interpolatedPoses.insert({ m.first, interpolated });
					}


					callback(time + t, frameDt, interpolatedPoses);
					interpolatedPoses.clear();
					//Sleep(frameDt);

					i++;
					if (i == 2)
						i = 0;

				} while (t < sleepTime);

				lastFrameTime = lastTime;
				isBoth = false;
			}
		

			//Переключение буферов позиций
			curPose++;
			if (curPose == 2)
				curPose = 0;

			isBoth = true;
		}


		AddDrive(poses[curPose], motor, pos);
		lastTime = time;
	}
