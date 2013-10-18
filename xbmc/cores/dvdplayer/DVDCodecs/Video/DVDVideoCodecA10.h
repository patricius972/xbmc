#pragma once

/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "guilib/Geometry.h"
#include "DVDVideoCodec.h"
#include "DVDStreamInfo.h"

extern "C" {
#include "cores/a10/libcedarx.h"
#include "cores/a10/DllLibcedarx.h"
};

class CDVDVideoCodecA10 : public CDVDVideoCodec
{
public:

  CDVDVideoCodecA10();
  virtual ~CDVDVideoCodecA10();

  /*
   * Open the decoder, returns true on success
   */
  bool Open(CDVDStreamInfo &hints, CDVDCodecOptions &options);

  /*
   * Dispose, Free all resources
   */
  void Dispose();

  /*
   * returns one or a combination of VC_ messages
   * pData and iSize can be NULL, this means we should flush the rest of the data.
   */
  int Decode(BYTE* pData, int iSize, double dts, double pts);

 /*
   * Reset the decoder.
   * Should be the same as calling Dispose and Open after each other
   */
  void Reset();

  /*
   * returns true if successfull
   * the data is valid until the next Decode call
   */
  bool GetPicture(DVDVideoPicture* pDvdVideoPicture);


  /*
   * returns true if successfull
   * the data is cleared to zero
   */
  /*-->super
  bool ClearPicture(DVDVideoPicture* pDvdVideoPicture);
  */

  /*
   * returns true if successfull
   * the data is valid until the next Decode call
   * userdata can be anything, for now we use it for closed captioning
   */
  /*-->super
  bool GetUserData(DVDVideoUserData* pDvdVideoUserData);
  */

  /*
   * will be called by video player indicating if a frame will eventually be dropped
   * codec can then skip actually decoding the data, just consume the data set picture headers
   */
  void SetDropState(bool bDrop);

  /*
   * returns the number of demuxer bytes in any internal buffers
   */
  /*-->super
  int GetDataSize(void);
  */

  /*
   * returns the time in seconds for demuxer bytes in any internal buffers
   */
  /*-->super
  virtual double GetTimeSize(void);
  */

  /*
   * set the type of filters that should be applied at decoding stage if possible
   */
  /*-->super
  unsigned int SetFilters(unsigned int filters);
  */

  /*
   *
   * should return codecs name
   */
  const char* GetName();

  /*
   *
   * How many packets should player remember, so codec
   * can recover should something cause it to flush
   * outside of players control
   */
  /*-->super
  virtual unsigned GetConvergeCount();
  */

  void FreePicture(void *pictpriv, cedarv_picture_t &pict);

private:

  bool DoOpen();

  //rendering
  bool m_hwrender;

  //decoding
  cedarx_info_t  m_info;
  float                 m_aspect;
  CDVDStreamInfo        m_hints;
  cedarx_decoder_t     *m_hcedarx;
  int                   m_hscaler;
  u8                   *m_yuvdata;
  DVDVideoPicture       m_picture;

protected:

  // bitstream to bytestream (Annex B) conversion support.
  bool bitstream_convert_init(void *in_extradata, int in_extrasize);
  bool bitstream_convert(BYTE* pData, int iSize, uint8_t **poutbuf, int *poutbuf_size);
  void bitstream_alloc_and_copy( uint8_t **poutbuf, int *poutbuf_size,
		const uint8_t *sps_pps, uint32_t sps_pps_size, const uint8_t *in, uint32_t in_size);

  typedef struct a10_bitstream_ctx {
	  uint8_t  length_size;
	  uint8_t  first_idr;
	  uint8_t *sps_pps_data;
	  uint32_t size;
  } a10_bitstream_ctx;

  uint32_t          m_sps_pps_size;
  a10_bitstream_ctx m_sps_pps_context;
  bool              m_convert_bitstream;
};

//////////////////////////////////////////////////////////////////////////

static DllLibA10decoder g_libbdv;
