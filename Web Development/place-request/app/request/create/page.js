'use client'
import {
    Alert,
    AlertIcon,
    AlertTitle,
    Card,
    Grid,
} from '@chakra-ui/react';
import { useSession } from 'next-auth/react';


export default function Page() {
    const { date: session } = useSession();
    const [title, setTitle] = useState('');
    const [description, setDescription] = useState('');
    const [destinationType, setDestinationType] = useState('')
    const [maxExpectedPrice, setMaxExpectedPrice] = useState(0);
    const [seekerExpiryDate, setSeekerExpiryDate] = useState('');

    const [isTitleInvalid, setIsTitleInvalid] = useState(false);
    const [isDescriptionInvalid, setIsDescriptionInvalid] = useState(false);
    const [isDestinationTypeInvalid, setIsDestinationTypeInvalid] = useState(false);
    const [isMaxExpectedPriceInvalid, setIsMaxExpectedPriceInvalid] = useState(false);
    const [isSeekerExpiryDateInvalid, setIsSeekerExpiryDateInvalid] = useState(false);



    return (
        <>
            {session ? (
                <>
                    <Card align='center' w='800px' h='500px'>
                        <CardHeader>
                            <Heading size='lg' >创建请求</Heading>
                        </CardHeader>

                        <CardBody>
                            <Grid templateRows='repeat(8, 1fr)' templateColumns='repeat(2, 1fr)' gap={1}>
                                <GridItem rowSpan={1} colSpan={1} p='4'>
                                    <Text>标题</Text>
                                    <Input
                                        value={title}
                                        onChange={(e) => setTitle(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isTitleInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                    <Text>真实姓名</Text>
                                    <Input
                                        value={realName}
                                        onChange={(e) => setRealName(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isRealNameInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                    <Text>密码</Text>
                                    <Input
                                        value={password}
                                        type={show ? 'text' : 'password'}
                                        onChange={(e) => setPassword(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isPasswordInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                    <Text>确认密码</Text>
                                    <Input
                                        value={confirmPassword}
                                        type={show ? 'text' : 'password'}
                                        onChange={(e) => setConfirmPassword(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isConfirmPasswordInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                    <Text>证件类型</Text>
                                    <Menu>
                                        <MenuButton w='full' as={Button} rightIcon={<ChevronDownIcon />} >
                                            <Flex justify='left'>
                                                {identityType}
                                            </Flex>
                                        </MenuButton>
                                        <MenuList>
                                            <MenuItem onClick={() => setIdentityType('居民身份证')}>居民身份证</MenuItem>
                                            <MenuItem onClick={() => setIdentityType('护照')}>护照</MenuItem>
                                        </MenuList>
                                    </Menu>
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                    <Text>证件号</Text>
                                    <Input
                                        value={identityNumber}
                                        onChange={(e) => setIdentityNumber(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isIdentityNumberInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                    <Text>电话号码</Text>
                                    <Input
                                        value={phoneNumber}
                                        onChange={(e) => setPhoneNumber(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isPhoneNumberInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={3} p='4'>
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={2} p='4'>
                                    <Text>国家</Text>
                                    <Input
                                        value={country}
                                        onChange={(e) => setCountry(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isCountryInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={2} p='4'>
                                    <Text>省份</Text>
                                    <Input
                                        value={province}
                                        onChange={(e) => setProvince(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isProvinceInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={1} colSpan={2} p='4'>
                                    <Text>市</Text>
                                    <Input
                                        value={city}
                                        onChange={(e) => setCity(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isCityInvalid}
                                    />
                                </GridItem>
                                <GridItem rowSpan={3} colSpan={6} p='4'>
                                    <Text>个人简介</Text>
                                    <Textarea
                                        value={introduction}
                                        h='200px'
                                        onChange={(e) => setIntroduction(e.target.value)}
                                        errorBorderColor='crimson'
                                        isInvalid={isIntroductionInvalid}
                                    />
                                </GridItem>
                            </Grid>
                        </CardBody>
                        <CardFooter>
                            <Button colorScheme='telegram' size='lg' onClick={() => handleSubmit()} >完成</Button>
                        </CardFooter>
                    </Card>
                </>
            ) : (
                <Card align='center'>
                    <Alert status='error'>
                        <AlertIcon />
                        <AlertTitle>请先登录！</AlertTitle>
                    </Alert >
                </Card >
            )
            }
        </>
    );
}