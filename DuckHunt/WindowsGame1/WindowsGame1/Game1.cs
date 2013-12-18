using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Duck;

namespace WindowsGame1
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D DuckTex;
        Texture2D m_Cursor;
        Duck.Duck[] Ducks;
        MouseState MS;
        MouseState PMS;
        Random m_rand;

        //Round Stuff
        int Wave;
        int Num_bullets;
        int WaveLength;
        int Score;
        int Round;


        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            m_rand = new Random();
            Wave = 0;
            Num_bullets = 3;
            WaveLength = 0;
            Score = 0;
            Round = 0;
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            Ducks = new Duck.Duck[10];
            for (int i = 0; i < 10; i++)
            {
                Ducks[i] = new Duck.Duck(m_rand);
                Ducks[i].Init(m_rand);
            }
                base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        SpriteFont Font1;
        Vector2 FontPos;
        Vector2 TitlePos;
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
            Font1 = Content.Load<SpriteFont>("SpriteFont1");
            // TODO: Load your game content here            
            FontPos = new Vector2(graphics.GraphicsDevice.Viewport.Width / 8,
                graphics.GraphicsDevice.Viewport.Height / 8);

            TitlePos = new Vector2(graphics.GraphicsDevice.Viewport.Width / 2,
                graphics.GraphicsDevice.Viewport.Height / 4);
            
            spriteBatch = new SpriteBatch(GraphicsDevice);
            DuckTex = new Texture2D(GraphicsDevice, 1, 1);
            m_Cursor = new Texture2D(GraphicsDevice, 1, 1);
            Color[] DuckCol = new Color[1];
            DuckCol[0] = Color.White;
            DuckTex.SetData(DuckCol);
            DuckCol[0] = Color.Black;
            m_Cursor.SetData(DuckCol);
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {

            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            if (Wave > 4)
            {
                bool bonus = true;
                for (int i = 0; i < 10; i++)
                {
                    bonus &= Ducks[i].getDead();
                }
                if (bonus)
                    Score += 1000;


                for (int i = 0; i < 10; i++)
                {
                    Ducks[i] = new Duck.Duck(m_rand);
                    Ducks[i].Init(m_rand);
                }

                Wave = 0;
                Round++;
            }

            if ((Ducks[Wave * 2].getDead() && Ducks[(Wave * 2) + 1].getDead()) || (((WaveLength - gameTime.TotalGameTime.Seconds) <= 0) && WaveLength != 0))
            {
                Wave++;
                WaveLength = 0;
                Num_bullets = 3;
            }


            if(WaveLength == 0)
                WaveLength = gameTime.TotalGameTime.Seconds + 12;

            if(WaveLength > 12)
                WaveLength = 12;

            if (WaveLength - gameTime.TotalGameTime.Seconds < 10)//ONLY PROCESS THE GAME WHILE THE ROUND IS COMMENCING!
            {


                MS = Mouse.GetState();
                if (MS.LeftButton == ButtonState.Pressed && PMS.LeftButton != ButtonState.Pressed)
                {
                    var mousePosition = new Point(MS.X, MS.Y);
                    if (Num_bullets > 0)
                    {
                        Fire(mousePosition);
                        Num_bullets--;
                    }
                }
                PMS = MS;


                for (int i = 0; i < 2; i++)
                {
                    if (!Ducks[i + Wave * 2].getDead())
                        Ducks[i + Wave * 2].Update(gameTime, m_rand,Round);
                }
               



            }

            // TODO: Add your update logic here

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Add your drawing code here
            spriteBatch.Begin();
            string output = "Score: ";
            output += Score;
            output += "\nWave: ";
            output += Wave;
            output += "\nRound: ";
            output += Round;
            output += "\nWave Time: ";
            output += (WaveLength - gameTime.TotalGameTime.Seconds).ToString();
            string Title;
            if (WaveLength - gameTime.TotalGameTime.Seconds >= 10)
                Title = "Inter Wave Rest";
            else
                Title = "BOX KILLING SPREE\n    EXTREME";



            // Find the center of the string
            Vector2 FontOrigin = Font1.MeasureString(output) / 2;
            // Draw the string
            spriteBatch.DrawString(Font1, output, FontPos, Color.White,
                0, FontOrigin, 1.0f, SpriteEffects.None, 0.5f);
            FontOrigin = Font1.MeasureString(Title) / 2;
            spriteBatch.DrawString(Font1, Title, TitlePos, Color.White,
                0, FontOrigin, 1.0f, SpriteEffects.None, 0.5f);

            Color DuckCol = Color.White;
            if (Wave < 5)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (Ducks[i + Wave * 2].getDead() != true)
                        spriteBatch.Draw(DuckTex, Ducks[i + Wave * 2].getHitbox(), DuckCol);
                }
            }





            DuckCol = Color.Black;
            Rectangle Rect;
            Rect.X = MS.X;
            Rect.Y = MS.Y;
            Rect.Width = 10;
            Rect.Height = 10;
            
            spriteBatch.Draw(m_Cursor, Rect, DuckCol);
            //spriteBatch.Draw(dummyTexture, leftBorder,   translucentRed);
            for (int i = 0; i < Num_bullets; i++)
            {
                Rectangle rect;
                rect.Height = 20;
                rect.Width = 10;
                rect.Y = 400;
                rect.X = 20 + i * 20;

                spriteBatch.Draw(m_Cursor, rect, DuckCol);
            }

            spriteBatch.End();



            base.Draw(gameTime);
        }


        protected void Fire(Point vec){
            for(int i = 0;i < 2; i++){
                if(Ducks[i+Wave] != null)
                    if (Ducks[i + Wave*2].getHitbox().Contains(vec))
                    {
                        Ducks[i + Wave*2].setDead(true);
                        Score += 100;
                    }
            }
        }
    }
}
