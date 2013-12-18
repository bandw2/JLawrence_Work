using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;


namespace Duck
{
    class Duck
    {
        Microsoft.Xna.Framework.Rectangle m_Hitbox;
        Vector2 Velocity;
        Vector2 Location;
        bool dead;

        public Duck(Random m_ran)
        {

            dead = false;
            m_Hitbox = new Rectangle();
            Velocity = new Vector2();
            m_Hitbox.X = m_ran.Next(200, 600);
            Location.X = (float)m_Hitbox.X;
            m_Hitbox.Y = 300;
            Location.Y = 300;
            Velocity.X = 0.0F;
            Velocity.Y = -10.0F;


            m_Hitbox.Height = 64;
            m_Hitbox.Width = 64;
        }

        public void Init(Random m_ran)
        {
            m_Hitbox.X = m_ran.Next(200, 600);

        }

        public void Update(GameTime gameTime, Random m_ran, int Round)
        {
            float Difficulty = (float)((float)(Round+1) * 1.1F);
            Velocity.X += (float)(m_ran.NextDouble() - 0.5)*5;
            Velocity.Y += (float)(m_ran.NextDouble() - 0.5);

            if (gameTime.ElapsedGameTime.Milliseconds != 0)
            {
                Location.X += (Velocity.X * Difficulty) / gameTime.ElapsedGameTime.Milliseconds;
                Location.Y += (Velocity.Y * Difficulty) / gameTime.ElapsedGameTime.Milliseconds;
            }


            m_Hitbox.X = (int)Location.X;
            m_Hitbox.Y = (int)Location.Y;

            if (Location.Y >= 600)
                Velocity.Y = -(Math.Abs(Velocity.Y));

            if(Location.X >= 800)
                Velocity.X = -(Math.Abs(Velocity.X));

            if(Location.X <= 0)
                Velocity.X = (Math.Abs(Velocity.Y));

        }

        public bool getDead()
        {
            return dead;
        }

        public void setDead(bool a_dead)
        {
            dead = a_dead;
        }

        public Rectangle getHitbox()
        {
            return m_Hitbox;
        }
    }
}
