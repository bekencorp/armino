//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <common/bk_include.h>
#include "psram_hal.h"
#include "psram_ll_macro_def.h"

void psram_hal_set_sf_reset(uint32_t value)
{
	psram_ll_set_sf_reset_value(value);
}

void psram_hal_set_cmd_reset(void)
{
	psram_ll_set_reg8_value(0x4);
}

void psram_hal_cmd_write(uint32_t addr, uint32_t value)
{
	psram_ll_set_write_address(addr);
	psram_ll_set_write_data(value);
	psram_ll_set_reg8_value(0x1);
	while(psram_ll_get_reg8_value() & 0x1);
}

uint32_t psram_hal_cmd_read(uint32_t addr)
{
	psram_ll_set_write_address(addr);
	psram_ll_set_reg8_value(0x2);
	while(psram_ll_get_reg8_value() & 0x2);

	return psram_ll_get_regb_value();
}

void psram_hal_set_transfer_mode(uint32_t value)
{
	psram_ll_set_reg4_wrap_config(value);
}


